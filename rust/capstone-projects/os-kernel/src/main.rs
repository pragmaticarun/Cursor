use clap::{Parser, Subcommand};
use serde::{Deserialize, Serialize};
use std::collections::{HashMap, VecDeque};
use std::sync::{Arc, Mutex};
use std::thread;
use std::time::{Duration, Instant};
use thiserror::Error;
use uuid::Uuid;

#[derive(Parser)]
#[command(name = "os-kernel")]
#[command(about = "A minimal OS kernel demonstrating memory management and process scheduling")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    /// Start the kernel simulation
    Start {
        #[arg(short, long, default_value = "1000")]
        memory_size: usize,
        #[arg(short, long, default_value = "4")]
        cpu_cores: usize,
    },
    /// Run kernel tests
    Test,
    /// Show kernel statistics
    Stats,
}

#[derive(Error, Debug)]
enum KernelError {
    #[error("Memory error: {0}")]
    MemoryError(String),
    #[error("Process error: {0}")]
    ProcessError(String),
    #[error("Scheduler error: {0}")]
    SchedulerError(String),
    #[error("System call error: {0}")]
    SystemCallError(String),
}

// Memory Management
#[derive(Debug, Clone, Serialize, Deserialize)]
struct MemoryBlock {
    id: Uuid,
    start: usize,
    size: usize,
    allocated: bool,
    process_id: Option<Uuid>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct MemoryManager {
    total_size: usize,
    blocks: Vec<MemoryBlock>,
    free_space: usize,
}

impl MemoryManager {
    fn new(total_size: usize) -> Self {
        let initial_block = MemoryBlock {
            id: Uuid::new_v4(),
            start: 0,
            size: total_size,
            allocated: false,
            process_id: None,
        };
        
        Self {
            total_size,
            blocks: vec![initial_block],
            free_space: total_size,
        }
    }
    
    fn allocate(&mut self, size: usize, process_id: Uuid) -> Result<Uuid, KernelError> {
        if size > self.free_space {
            return Err(KernelError::MemoryError("Not enough memory".to_string()));
        }
        
        // Find first fit
        for (i, block) in self.blocks.iter_mut().enumerate() {
            if !block.allocated && block.size >= size {
                if block.size == size {
                    // Perfect fit
                    block.allocated = true;
                    block.process_id = Some(process_id);
                    self.free_space -= size;
                    return Ok(block.id);
                } else {
                    // Split block
                    let new_block = MemoryBlock {
                        id: Uuid::new_v4(),
                        start: block.start + size,
                        size: block.size - size,
                        allocated: false,
                        process_id: None,
                    };
                    
                    block.size = size;
                    block.allocated = true;
                    block.process_id = Some(process_id);
                    
                    self.blocks.insert(i + 1, new_block);
                    self.free_space -= size;
                    return Ok(block.id);
                }
            }
        }
        
        Err(KernelError::MemoryError("No suitable block found".to_string()))
    }
    
    fn deallocate(&mut self, block_id: Uuid) -> Result<(), KernelError> {
        let block_index = self.blocks.iter().position(|b| b.id == block_id)
            .ok_or_else(|| KernelError::MemoryError("Block not found".to_string()))?;
        
        let block_size = self.blocks[block_index].size;
        self.blocks[block_index].allocated = false;
        self.blocks[block_index].process_id = None;
        self.free_space += block_size;
        
        // Merge with adjacent free blocks
        self.merge_free_blocks();
        
        Ok(())
    }
    
    fn merge_free_blocks(&mut self) {
        let mut i = 0;
        while i < self.blocks.len() - 1 {
            let current = &self.blocks[i];
            let next = &self.blocks[i + 1];
            
            if !current.allocated && !next.allocated && 
               current.start + current.size == next.start {
                // Merge blocks
                self.blocks[i].size += next.size;
                self.blocks.remove(i + 1);
            } else {
                i += 1;
            }
        }
    }
    
    fn get_stats(&self) -> MemoryStats {
        let allocated_blocks = self.blocks.iter().filter(|b| b.allocated).count();
        let free_blocks = self.blocks.iter().filter(|b| !b.allocated).count();
        
        MemoryStats {
            total_size: self.total_size,
            free_space: self.free_space,
            allocated_space: self.total_size - self.free_space,
            total_blocks: self.blocks.len(),
            allocated_blocks,
            free_blocks,
        }
    }
}

#[derive(Debug, Serialize, Deserialize)]
struct MemoryStats {
    total_size: usize,
    free_space: usize,
    allocated_space: usize,
    total_blocks: usize,
    allocated_blocks: usize,
    free_blocks: usize,
}

// Process Management
#[derive(Debug, Clone, Serialize, Deserialize)]
enum ProcessState {
    Ready,
    Running,
    Blocked,
    Terminated,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Process {
    id: Uuid,
    name: String,
    priority: u8,
    state: ProcessState,
    memory_blocks: Vec<Uuid>,
    cpu_time: Duration,
    creation_time: Instant,
    last_run_time: Option<Instant>,
}

impl Process {
    fn new(name: String, priority: u8) -> Self {
        Self {
            id: Uuid::new_v4(),
            name,
            priority,
            state: ProcessState::Ready,
            memory_blocks: Vec::new(),
            cpu_time: Duration::new(0, 0),
            creation_time: Instant::now(),
            last_run_time: None,
        }
    }
    
    fn allocate_memory(&mut self, memory_manager: &mut MemoryManager, size: usize) -> Result<Uuid, KernelError> {
        let block_id = memory_manager.allocate(size, self.id)?;
        self.memory_blocks.push(block_id);
        Ok(block_id)
    }
    
    fn deallocate_memory(&mut self, memory_manager: &mut MemoryManager, block_id: Uuid) -> Result<(), KernelError> {
        memory_manager.deallocate(block_id)?;
        self.memory_blocks.retain(|&id| id != block_id);
        Ok(())
    }
    
    fn terminate(&mut self, memory_manager: &mut MemoryManager) -> Result<(), KernelError> {
        // Deallocate all memory
        for &block_id in &self.memory_blocks.clone() {
            memory_manager.deallocate(block_id)?;
        }
        self.memory_blocks.clear();
        
        self.state = ProcessState::Terminated;
        Ok(())
    }
}

// CPU Scheduler
#[derive(Debug, Clone, Serialize, Deserialize)]
enum SchedulingAlgorithm {
    RoundRobin,
    Priority,
    ShortestJobFirst,
}

struct CPUScheduler {
    algorithm: SchedulingAlgorithm,
    ready_queue: VecDeque<Uuid>,
    running_process: Option<Uuid>,
    time_quantum: Duration,
    current_time_slice: Duration,
}

impl CPUScheduler {
    fn new(algorithm: SchedulingAlgorithm, time_quantum: Duration) -> Self {
        Self {
            algorithm,
            ready_queue: VecDeque::new(),
            running_process: None,
            time_quantum,
            current_time_slice: Duration::new(0, 0),
        }
    }
    
    fn add_process(&mut self, process_id: Uuid) {
        self.ready_queue.push_back(process_id);
    }
    
    fn schedule(&mut self, processes: &mut HashMap<Uuid, Process>) -> Option<Uuid> {
        match self.algorithm {
            SchedulingAlgorithm::RoundRobin => self.schedule_round_robin(processes),
            SchedulingAlgorithm::Priority => self.schedule_priority(processes),
            SchedulingAlgorithm::ShortestJobFirst => self.schedule_sjf(processes),
        }
    }
    
    fn schedule_round_robin(&mut self, processes: &mut HashMap<Uuid, Process>) -> Option<Uuid> {
        if let Some(current_id) = self.running_process {
            if self.current_time_slice >= self.time_quantum {
                // Time slice expired, move to ready queue
                if let Some(process) = processes.get_mut(&current_id) {
                    process.state = ProcessState::Ready;
                }
                self.ready_queue.push_back(current_id);
                self.current_time_slice = Duration::new(0, 0);
            }
        }
        
        // Get next process from ready queue
        if let Some(next_id) = self.ready_queue.pop_front() {
            if let Some(process) = processes.get_mut(&next_id) {
                process.state = ProcessState::Running;
                process.last_run_time = Some(Instant::now());
            }
            self.running_process = Some(next_id);
            return Some(next_id);
        }
        
        None
    }
    
    fn schedule_priority(&mut self, processes: &mut HashMap<Uuid, Process>) -> Option<Uuid> {
        // Sort ready queue by priority (higher priority first)
        let mut ready_processes: Vec<_> = self.ready_queue.iter().collect();
        ready_processes.sort_by(|a, b| {
            let priority_a = processes.get(a).map(|p| p.priority).unwrap_or(0);
            let priority_b = processes.get(b).map(|p| p.priority).unwrap_or(0);
            priority_b.cmp(&priority_a) // Higher priority first
        });
        
        self.ready_queue.clear();
        for &process_id in &ready_processes {
            self.ready_queue.push_back(process_id);
        }
        
        self.schedule_round_robin(processes)
    }
    
    fn schedule_sjf(&mut self, processes: &mut HashMap<Uuid, Process>) -> Option<Uuid> {
        // Sort ready queue by CPU time (shorter jobs first)
        let mut ready_processes: Vec<_> = self.ready_queue.iter().collect();
        ready_processes.sort_by(|a, b| {
            let time_a = processes.get(a).map(|p| p.cpu_time).unwrap_or(Duration::new(0, 0));
            let time_b = processes.get(b).map(|p| p.cpu_time).unwrap_or(Duration::new(0, 0));
            time_a.cmp(&time_b)
        });
        
        self.ready_queue.clear();
        for &process_id in &ready_processes {
            self.ready_queue.push_back(process_id);
        }
        
        self.schedule_round_robin(processes)
    }
    
    fn tick(&mut self, processes: &mut HashMap<Uuid, Process>) {
        self.current_time_slice += Duration::from_millis(1);
        
        if let Some(process_id) = self.running_process {
            if let Some(process) = processes.get_mut(&process_id) {
                process.cpu_time += Duration::from_millis(1);
            }
        }
    }
}

// System Calls
#[derive(Debug, Clone, Serialize, Deserialize)]
enum SystemCall {
    Fork { name: String, priority: u8 },
    Exit { process_id: Uuid },
    AllocateMemory { size: usize },
    DeallocateMemory { block_id: Uuid },
    GetProcessInfo { process_id: Uuid },
    GetMemoryInfo,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct SystemCallResult {
    success: bool,
    data: Option<serde_json::Value>,
    error: Option<String>,
}

// Kernel
struct Kernel {
    memory_manager: Arc<Mutex<MemoryManager>>,
    processes: Arc<Mutex<HashMap<Uuid, Process>>>,
    scheduler: Arc<Mutex<CPUScheduler>>,
    cpu_cores: usize,
}

impl Kernel {
    fn new(memory_size: usize, cpu_cores: usize) -> Self {
        Self {
            memory_manager: Arc::new(Mutex::new(MemoryManager::new(memory_size))),
            processes: Arc::new(Mutex::new(HashMap::new())),
            scheduler: Arc::new(Mutex::new(CPUScheduler::new(
                SchedulingAlgorithm::RoundRobin,
                Duration::from_millis(100)
            ))),
            cpu_cores,
        }
    }
    
    fn handle_system_call(&self, call: SystemCall) -> SystemCallResult {
        match call {
            SystemCall::Fork { name, priority } => {
                let mut processes = self.processes.lock().unwrap();
                let mut process = Process::new(name, priority);
                
                // Allocate initial memory
                let mut memory_manager = self.memory_manager.lock().unwrap();
                if let Err(e) = process.allocate_memory(&mut memory_manager, 1024) {
                    return SystemCallResult {
                        success: false,
                        data: None,
                        error: Some(format!("Memory allocation failed: {}", e)),
                    };
                }
                
                let process_id = process.id;
                processes.insert(process_id, process);
                
                // Add to scheduler
                let mut scheduler = self.scheduler.lock().unwrap();
                scheduler.add_process(process_id);
                
                SystemCallResult {
                    success: true,
                    data: Some(serde_json::to_value(process_id).unwrap()),
                    error: None,
                }
            }
            SystemCall::Exit { process_id } => {
                let mut processes = self.processes.lock().unwrap();
                let mut memory_manager = self.memory_manager.lock().unwrap();
                
                if let Some(mut process) = processes.remove(&process_id) {
                    if let Err(e) = process.terminate(&mut memory_manager) {
                        return SystemCallResult {
                            success: false,
                            data: None,
                            error: Some(format!("Process termination failed: {}", e)),
                        };
                    }
                    
                    SystemCallResult {
                        success: true,
                        data: None,
                        error: None,
                    }
                } else {
                    SystemCallResult {
                        success: false,
                        data: None,
                        error: Some("Process not found".to_string()),
                    }
                }
            }
            SystemCall::AllocateMemory { size } => {
                let mut memory_manager = self.memory_manager.lock().unwrap();
                let mut processes = self.processes.lock().unwrap();
                
                // Find a running process to allocate memory for
                let running_process = processes.values()
                    .find(|p| matches!(p.state, ProcessState::Running))
                    .map(|p| p.id);
                
                if let Some(process_id) = running_process {
                    if let Some(process) = processes.get_mut(&process_id) {
                        match process.allocate_memory(&mut memory_manager, size) {
                            Ok(block_id) => SystemCallResult {
                                success: true,
                                data: Some(serde_json::to_value(block_id).unwrap()),
                                error: None,
                            },
                            Err(e) => SystemCallResult {
                                success: false,
                                data: None,
                                error: Some(format!("Memory allocation failed: {}", e)),
                            },
                        }
                    } else {
                        SystemCallResult {
                            success: false,
                            data: None,
                            error: Some("Process not found".to_string()),
                        }
                    }
                } else {
                    SystemCallResult {
                        success: false,
                        data: None,
                        error: Some("No running process".to_string()),
                    }
                }
            }
            SystemCall::DeallocateMemory { block_id } => {
                let mut memory_manager = self.memory_manager.lock().unwrap();
                let mut processes = self.processes.lock().unwrap();
                
                // Find process that owns this memory block
                let owner_process = processes.values_mut()
                    .find(|p| p.memory_blocks.contains(&block_id));
                
                if let Some(process) = owner_process {
                    match process.deallocate_memory(&mut memory_manager, block_id) {
                        Ok(_) => SystemCallResult {
                            success: true,
                            data: None,
                            error: None,
                        },
                        Err(e) => SystemCallResult {
                            success: false,
                            data: None,
                            error: Some(format!("Memory deallocation failed: {}", e)),
                        },
                    }
                } else {
                    SystemCallResult {
                        success: false,
                        data: None,
                        error: Some("Memory block not found".to_string()),
                    }
                }
            }
            SystemCall::GetProcessInfo { process_id } => {
                let processes = self.processes.lock().unwrap();
                if let Some(process) = processes.get(&process_id) {
                    SystemCallResult {
                        success: true,
                        data: Some(serde_json::to_value(process).unwrap()),
                        error: None,
                    }
                } else {
                    SystemCallResult {
                        success: false,
                        data: None,
                        error: Some("Process not found".to_string()),
                    }
                }
            }
            SystemCall::GetMemoryInfo => {
                let memory_manager = self.memory_manager.lock().unwrap();
                let stats = memory_manager.get_stats();
                SystemCallResult {
                    success: true,
                    data: Some(serde_json::to_value(stats).unwrap()),
                    error: None,
                }
            }
        }
    }
    
    fn run_scheduler(&self) {
        let processes = self.processes.clone();
        let scheduler = self.scheduler.clone();
        
        thread::spawn(move || {
            loop {
                let mut processes_guard = processes.lock().unwrap();
                let mut scheduler_guard = scheduler.lock().unwrap();
                
                scheduler_guard.tick(&mut processes_guard);
                scheduler_guard.schedule(&mut processes_guard);
                
                drop(processes_guard);
                drop(scheduler_guard);
                
                thread::sleep(Duration::from_millis(1));
            }
        });
    }
    
    fn get_kernel_stats(&self) -> KernelStats {
        let memory_manager = self.memory_manager.lock().unwrap();
        let processes = self.processes.lock().unwrap();
        let scheduler = self.scheduler.lock().unwrap();
        
        let memory_stats = memory_manager.get_stats();
        let process_count = processes.len();
        let running_count = processes.values()
            .filter(|p| matches!(p.state, ProcessState::Running))
            .count();
        
        KernelStats {
            memory_stats,
            process_count,
            running_count,
            cpu_cores: self.cpu_cores,
            scheduler_algorithm: format!("{:?}", scheduler.algorithm),
        }
    }
}

#[derive(Debug, Serialize, Deserialize)]
struct KernelStats {
    memory_stats: MemoryStats,
    process_count: usize,
    running_count: usize,
    cpu_cores: usize,
    scheduler_algorithm: String,
}

fn run_kernel_tests() -> Result<(), Box<dyn std::error::Error>> {
    println!("Running kernel tests...");
    
    // Test memory management
    let mut memory_manager = MemoryManager::new(1024 * 1024); // 1MB
    let test_process_id = Uuid::new_v4();
    
    // Allocate memory
    let block_id = memory_manager.allocate(1024, test_process_id)?;
    println!("✓ Memory allocation test passed");
    
    // Check stats
    let stats = memory_manager.get_stats();
    assert_eq!(stats.allocated_space, 1024);
    println!("✓ Memory stats test passed");
    
    // Deallocate memory
    memory_manager.deallocate(block_id)?;
    let stats = memory_manager.get_stats();
    assert_eq!(stats.allocated_space, 0);
    println!("✓ Memory deallocation test passed");
    
    // Test process management
    let mut process = Process::new("test_process".to_string(), 5);
    let mut memory_manager = MemoryManager::new(1024 * 1024);
    
    let block_id = process.allocate_memory(&mut memory_manager, 2048)?;
    assert_eq!(process.memory_blocks.len(), 1);
    println!("✓ Process memory allocation test passed");
    
    process.deallocate_memory(&mut memory_manager, block_id)?;
    assert_eq!(process.memory_blocks.len(), 0);
    println!("✓ Process memory deallocation test passed");
    
    // Test scheduler
    let mut scheduler = CPUScheduler::new(SchedulingAlgorithm::RoundRobin, Duration::from_millis(100));
    let mut processes = HashMap::new();
    
    let process1 = Process::new("proc1".to_string(), 1);
    let process2 = Process::new("proc2".to_string(), 2);
    
    let id1 = process1.id;
    let id2 = process2.id;
    
    processes.insert(id1, process1);
    processes.insert(id2, process2);
    
    scheduler.add_process(id1);
    scheduler.add_process(id2);
    
    let scheduled = scheduler.schedule(&mut processes);
    assert!(scheduled.is_some());
    println!("✓ Process scheduling test passed");
    
    println!("All kernel tests passed! ✓");
    Ok(())
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let cli = Cli::parse();
    
    match cli.command {
        Commands::Start { memory_size, cpu_cores } => {
            println!("Starting OS Kernel simulation...");
            println!("Memory size: {} bytes", memory_size);
            println!("CPU cores: {}", cpu_cores);
            
            let kernel = Kernel::new(memory_size, cpu_cores);
            kernel.run_scheduler();
            
            // Create some test processes
            let fork_call = SystemCall::Fork {
                name: "init".to_string(),
                priority: 1,
            };
            let result = kernel.handle_system_call(fork_call);
            println!("Created init process: {:?}", result);
            
            let fork_call = SystemCall::Fork {
                name: "shell".to_string(),
                priority: 2,
            };
            let result = kernel.handle_system_call(fork_call);
            println!("Created shell process: {:?}", result);
            
            // Run for a while
            for i in 0..10 {
                thread::sleep(Duration::from_millis(100));
                
                if i % 5 == 0 {
                    let stats = kernel.get_kernel_stats();
                    println!("Kernel stats: {:?}", stats);
                }
            }
            
            println!("Kernel simulation completed");
        }
        Commands::Test => {
            run_kernel_tests()?;
        }
        Commands::Stats => {
            let kernel = Kernel::new(1024 * 1024, 4);
            let stats = kernel.get_kernel_stats();
            println!("Kernel Statistics:");
            println!("{}", serde_json::to_string_pretty(&stats)?);
        }
    }
    
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_memory_allocation() {
        let mut memory_manager = MemoryManager::new(1024);
        let process_id = Uuid::new_v4();
        
        let block_id = memory_manager.allocate(512, process_id).unwrap();
        assert_eq!(memory_manager.free_space, 512);
        
        memory_manager.deallocate(block_id).unwrap();
        assert_eq!(memory_manager.free_space, 1024);
    }

    #[test]
    fn test_process_creation() {
        let process = Process::new("test".to_string(), 5);
        assert_eq!(process.name, "test");
        assert_eq!(process.priority, 5);
        assert!(matches!(process.state, ProcessState::Ready));
    }

    #[test]
    fn test_scheduler() {
        let mut scheduler = CPUScheduler::new(SchedulingAlgorithm::RoundRobin, Duration::from_millis(100));
        let mut processes = HashMap::new();
        
        let process = Process::new("test".to_string(), 1);
        let process_id = process.id;
        processes.insert(process_id, process);
        
        scheduler.add_process(process_id);
        let scheduled = scheduler.schedule(&mut processes);
        
        assert!(scheduled.is_some());
        assert_eq!(scheduled.unwrap(), process_id);
    }
}
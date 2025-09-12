use criterion::{criterion_group, criterion_main, Criterion};
use high_performance_parser::*;

fn benchmark_json_parsing(c: &mut Criterion) {
    let test_data = generate_test_json(10000);
    
    c.bench_function("json_parse_sequential", |b| {
        b.iter(|| {
            JsonParser::parse_content_fast(&test_data)
        })
    });
    
    c.bench_function("json_parse_parallel", |b| {
        b.iter(|| {
            JsonParser::parse_parallel(&test_data)
        })
    });
}

fn benchmark_csv_parsing(c: &mut Criterion) {
    let test_data = generate_test_csv(10000);
    
    c.bench_function("csv_parse_sequential", |b| {
        b.iter(|| {
            CsvParser::parse_content_fast(&test_data)
        })
    });
    
    c.bench_function("csv_parse_parallel", |b| {
        b.iter(|| {
            CsvParser::parse_parallel(&test_data)
        })
    });
}

fn benchmark_log_parsing(c: &mut Criterion) {
    let test_data = generate_test_logs(10000);
    
    c.bench_function("log_parse_sequential", |b| {
        b.iter(|| {
            LogParser::parse_content_fast(&test_data, None)
        })
    });
    
    c.bench_function("log_parse_parallel", |b| {
        b.iter(|| {
            LogParser::parse_parallel(&test_data, None)
        })
    });
}

criterion_group!(benches, benchmark_json_parsing, benchmark_csv_parsing, benchmark_log_parsing);
criterion_main!(benches);
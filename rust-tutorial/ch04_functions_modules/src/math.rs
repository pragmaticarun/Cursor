pub fn add(a: i64, b: i64) -> i64 { a + b }

pub fn sum<T: IntoIterator<Item = i64>>(nums: T) -> i64 {
    nums.into_iter().sum()
}


static LANGUAGE: &str = "RS";
const THRESHOLD: i32 = 10;

fn is_b(n: i32) -> bool {
    n > THRESHOLD
}

pub fn constants() {
    let n = 20;
    println!("this is {}", LANGUAGE);
    println!("the threahold is {}", THRESHOLD);
    println!("{} is {}", n, if is_b(n) { "big" } else { "small" });
}

// use of uninitialized variable bindings is
// undefined behavior

// saturating cast from rust 1.45

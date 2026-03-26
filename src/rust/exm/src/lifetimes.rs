#![allow(dead_code)]
fn print_refs<'a, 'b>(x: &'a i32, y: &'b i32) {
    println!("x {}, y {}", x, y);
}

// mutable references lifetimes
fn add_one<'a>(x: &'a mut i32) {
    *x += 1
}

pub fn lifetimes() {
    let (fo, nin) = (4, 9);
    print_refs(&fo, &nin);
}

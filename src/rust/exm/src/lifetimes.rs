#![allow(dead_code)]
fn print_refs<'a, 'b>(x: &'a i32, y: &'b i32) {
    println!("x {}, y {}", x, y);
}

pub fn lifetimes() {
    let (fo, nin) = (4, 9);
    print_refs(&fo, &nin);
}

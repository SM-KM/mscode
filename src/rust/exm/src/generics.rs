use std::fmt::{Debug, Display};

trait DoubleDrop<T> {
    fn double_drop(self, _: T);
}

impl<T, U> DoubleDrop<T> for U {
    // it will take ownership and deallocated the passed
    // parameters
    fn double_drop(self, _: T) {}
}

trait HasArea {
    fn area(&self) -> f64;
}

impl HasArea for Rectangle {
    fn area(&self) -> f64 {
        self.length * self.height
    }
}

fn area<T: HasArea>(t: &T) -> f64 {
    t.area()
}

// Multiple bounds
fn compare_prints<T: Debug + Display>(t: &T) {}
fn compare_types<T: Debug, U: Debug>(t: &T, u: &U) {}

trait PrintOption {
    fn print_option(self);
}

// is way clearer that using multiple bounds for the types
impl<T> PrintOption for T
where
    Option<T>: Debug,
{
    fn print_option(self) {}
}

#[derive(Debug)]
struct Rectangle {
    length: f64,
    height: f64,
}

struct Empty;
struct Null;

// Bounds
// to specify that the types are going to
// be bound to the specific type
fn printer<T: Display>(t: T) {
    println!("{}", t);
}

pub fn generics() {
    let empty = Empty;
    let null = Null;
    empty.double_drop(null);
}

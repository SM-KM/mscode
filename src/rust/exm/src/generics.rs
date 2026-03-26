#![allow(dead_code)]
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
fn compare_prints<T: Debug + Display>(_t: &T) {}
fn compare_types<T: Debug, U: Debug>(_t: &T, _u: &U) {}

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

// associated types

fn difference<C: Contains>(_container: &C) -> i32 {
    4
}

// associated types example
struct Container(i32, i32);

trait Contains {
    type A;
    type B;

    fn contains(&self, _: &Self::A, _: &Self::B) -> bool;
    fn first(&self) -> i32;
    fn last(&self) -> i32;
}

impl Contains for Container {
    type A = i32;
    type B = i32;

    fn contains(&self, n1: &Self::A, n2: &Self::B) -> bool {
        (&self.0 == n1) && (&self.1 == n2)
    }
    fn first(&self) -> i32 {
        self.0
    }
    fn last(&self) -> i32 {
        self.1
    }
}

pub fn ass_types() {}

pub fn generics() {
    let empty = Empty;
    let null = Null;
    empty.double_drop(null);
}

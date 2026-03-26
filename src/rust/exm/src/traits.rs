#![allow(dead_code)]
trait Animal {}
struct Bee {}
impl Animal for Bee {}

fn rnd_anim(_dn: f64) -> Box<dyn Animal> {
    Box::new(Bee {})
}

// implementing the traits inside core::ops, allows to
// perform operator overloading

// iterators
struct Fibonacci {
    curr: u32,
    next: u32,
}

impl Iterator for Fibonacci {
    type Item = u32;
    fn next(&mut self) -> Option<Self::Item> {
        let current = self.curr;
        self.curr = self.next;
        self.next = current + self.next;
        Some(current)
    }
}

// impl trait
// fn parse_csv_docs<R: std::io::BufRead>(_src: R) {}, turn this into
// using impl, something to avoid the extra generic R
fn parse_csv_docs(_src: impl std::io::BufRead) {}

use std::iter;
use std::vec::IntoIter;

// it helps to reduce the signature in some cases a lot
fn combine_vecs_explicit_return(
    v: Vec<i32>,
    u: Vec<i32>,
) -> iter::Cycle<iter::Chain<IntoIter<i32>, IntoIter<i32>>> {
    v.into_iter().chain(u.into_iter()).cycle()
}

// turn it into something like:
fn combine_vecs(v: Vec<i32>, u: Vec<i32>) -> impl Iterator<Item = i32> {
    v.into_iter().chain(u.into_iter()).cycle()
}

// allows to return closures without having to allocate in the heap first
fn make_cls(y: i32) -> impl Fn(i32) -> i32 {
    move |x: i32| x + y
}

// kind of inheritance, supertraits
trait Person {
    fn name(&self) -> String;
}

trait Student: Person {
    fn uni(&self) -> String;
}

pub fn traits() {}

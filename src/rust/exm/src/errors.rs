#![allow(dead_code)]

use std::num::ParseIntError;
#[cfg(panic = "unwind")]
fn ah() {
    println!("Spit it out!!!!");
}

#[cfg(not(panic = "unwind"))]
fn ah() {
    println!("Spit it out!!!!");
}

fn drink(beverage: Option<&str>) {
    let inside = beverage.unwrap();
    if inside == "lemonade" {
        panic!("aAAaaAaaaAAAA")
    };
    println!("Love it, {}!", inside);
}

// option and unwrap
fn give_adlt(drink: Option<&str>) {
    match drink {
        Some("limonade") => println!("too sugary"),
        Some(inner) => println!("{}? nice", inner),
        None => println!("Nothing?, fuk u"),
    }
}

fn n_bit(curr_a: Option<u8>) -> Option<String> {
    let cur: u8 = curr_a? + 1;
    Some(format!("Next: {}", cur))
}

struct Person {
    job: Option<Job>,
}

#[derive(Clone, Copy)]
struct Job {
    phone_number: Option<PhoneNumber>,
}

#[derive(Clone, Copy)]
struct PhoneNumber {
    area_code: Option<u8>,
    number: u32,
}

impl Person {
    fn work_area_code(&self) -> Option<u8> {
        self.job?.phone_number?.area_code
    }
}

// conbinators = map
enum Food {
    Apple,
    Carrot,
    Tomato,
}

struct Peeled(Food);
struct Cooked(Food);
struct Chopped(Food);

fn process(food: Option<Food>) -> Option<Cooked> {
    food.map(|f| Peeled(f)).map(|Peeled(f)| Chopped(f)).map(|Chopped(f)| Cooked(f))
}

#[derive(Debug)]
enum Fruit {
    Apple,
    Orange,
    Banana,
    Kiwi,
    Lemon,
}

// type AliasedResult<T> = Result<T, ParseIntError>;
// fn mult(first: &str, second: &str) -> AliasedResult<i32> {
//     // Early returns
//     let _first_number = first.parse::<i32>()?;
//     let _second_number = second.parse::<i32>()?;
//     Ok(_first_number + _second_number)
// }

use std::fmt;
type Result<T> = std::result::Result<T, DoubleErr>;

#[derive(Debug, Clone)]
struct DoubleErr;

impl fmt::Display for DoubleErr {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Invalid first item")
    }
}

fn double_first(vec: Vec<&str>) -> Result<i32> {
    vec.first()
        .ok_or(DoubleErr)
        .and_then(|s| s.parse::<i32>().map_err(|_| DoubleErr).map(|i| 2 * i))
}

pub fn errors() {
    let numbers = vec!["42", "93", "18"];
    let empty = vec![];
    let strings = vec!["tofu", "93", "18"];

    print!("{:?}", double_first(numbers));
    print!("{:?}", double_first(empty));
    print!("{:?}", double_first(strings));
}

#![allow(dead_code)]

use std::convert::From;
use std::convert::Into;
use std::convert::TryFrom;
use std::convert::TryInto;
use std::fmt;

#[derive(Debug)]
struct Number {
    value: i32,
}

#[derive(Debug, PartialEq)]
struct EvenNumber(i32);

impl TryFrom<i32> for EvenNumber {
    type Error = ();
    fn try_from(value: i32) -> Result<Self, Self::Error> {
        if value % 2 == 0 { Ok(EvenNumber(value)) } else { Err(()) }
    }
}

impl From<i32> for Number {
    fn from(val: i32) -> Self {
        Number { value: val }
    }
}

// Strings conversions
struct Circle {
    rad: i32,
}

impl fmt::Display for Circle {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Circle of rad {}", self.rad)
    }
}

pub fn conversions() {
    let int = 5;
    let num = Number::from(30);
    let _m: Number = int.into();
    println!("{:?}", num);

    assert_eq!(EvenNumber::try_from(8), Ok(EvenNumber(8)));
    assert_eq!(EvenNumber::try_from(5), Err(()));

    let _result: Result<EvenNumber, ()> = 8i32.try_into();
    let _result: Result<EvenNumber, ()> = 5i32.try_into();

    let circle = Circle { rad: 6 };
    println!("{}", circle.to_string());
}

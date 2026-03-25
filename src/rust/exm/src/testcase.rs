enum List {
    // a Box is basically a smart pointer wrapper
    // for a type T
    Cons(u32, Box<List>),
    Nil,
}

impl List {
    fn new() -> List {
        List::Nil
    }
    fn prepend(self, elem: u32) -> List {
        List::Cons(elem, Box::new(self))
    }

    fn len(&self) -> u32 {
        match *self {
            List::Cons(_, ref tail) => 1 + tail.len(),
            List::Nil => 0,
        }
    }
    fn stringify(&self) -> String {
        match *self {
            List::Cons(head, ref tail) => {
                format!("{}, {}", head, tail.stringify())
            }
            List::Nil => format!("Nil"),
        }
    }
}

pub fn testcases() {
    let mut list = List::new();
    list = list.prepend(1);
    list = list.prepend(2);
    list = list.prepend(3);

    println!("len of: {}", list.len());
    println!("{}", list.stringify());
}

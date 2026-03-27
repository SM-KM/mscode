use std::{
    io, path,
    sync::mpsc::{Receiver, Sender},
};

// threads
const NTHREADS: u32 = 10;

pub fn stdlib() {
    let data = "86967897737416471853297327050364959
11861322575564723963297542624962850
70856234701860851907960690014725639
38397966707106094172783238747669219
52380795257888236525459303330302837
58495327135744041048897885734297812
69920216438980873548808413720956532
16278424637452589860345374828574668";

    let mut children = vec![];
    let chunked_data = data.split_whitespace();

    for (_, data_seg) in chunked_data.enumerate() {
        children.push(std::thread::spawn(move || -> u32 {
            let res = data_seg.chars().map(|c| c.to_digit(10).expect("should be a digit")).sum();
            res
        }));
    }

    let final_result = children.into_iter().map(|c| c.join().unwrap()).sum::<u32>();
    println!("Result {}", final_result);
}

// chanenels
pub fn channels() {
    let (tx, rx): (Sender<i32>, Receiver<i32>) = std::sync::mpsc::channel();
    let mut children = Vec::new();

    for id in 0..NTHREADS {
        let thread_tx = tx.clone();
        let child = std::thread::spawn(move || {
            thread_tx.send(id as i32).unwrap();
            println!("thread {}", id);
        });
        children.push(child);
    }

    let mut ids = Vec::with_capacity(NTHREADS as usize);
    for _ in 0..NTHREADS {
        ids.push(rx.recv());
    }

    for child in children {
        child.join().expect("oops! the child thread panicked");
    }

    println!("{:?}", ids);
}

pub fn path() {}

static LOREM_IPSUM: &str =
    "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod
tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non
proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
";

pub fn fileio() {
    use std::fs::File;
    use std::io::prelude::*;
    use std::path::Path;

    let path = Path::new("hello.txt");
    let display = path.display();

    let mut file = match File::create(&path) {
        Ok(file) => file,
        Err(why) => panic!("could not create the file: {}", why),
    };

    match file.write_all(LOREM_IPSUM.as_bytes()) {
        Ok(_) => print!("wrote into: {}", display),
        Err(why) => panic!("couldn't write {}: {}", display, why),
    }
}

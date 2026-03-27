use std::{
    fs::{File, OpenOptions, read_to_string},
    io::{self, BufRead, Read, Write},
    path::Path,
    process::{Command, Stdio},
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

// naive approach
fn read_lines_nv(filename: &str) -> Vec<String> {
    read_to_string(filename).unwrap().lines().map(String::from).collect()
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<std::fs::File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

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

    // match file.write_all(LOREM_IPSUM.as_bytes()) {
    //     Ok(_) => print!("wrote into: {}", display),
    //     Err(why) => panic!("couldn't write {}: {}", display, why),
    // }

    if let Ok(lines) = read_lines("./hosts.txt") {
        for line in lines.map_while(Result::ok) {
            println!("{}", line)
        }
    }
}

pub fn child_process() {
    use std::process::Command;
    let output = Command::new("rustc").arg("--version").output().unwrap_or_else(|e| {
        panic!("failed to exec {}", e);
    });

    if output.status.success() {
        let s = String::from_utf8_lossy(&output.stdout);
        print!("rustc worked: {}", s);
    } else {
        let s = String::from_utf8_lossy(&output.stderr);
        print!("rustc failed: {}", s);
    }
}

static PANGRAM: &'static str = "the quick brown fox jumps over the lazy dog\n";
pub fn pipes() {
    let mut cmd = Command::new("wc");
    let process = match cmd.stdin(Stdio::piped()).stdout(Stdio::piped()).spawn() {
        Ok(process) => process,
        Err(why) => panic!("could not spawn: {}", why),
    };

    match process.stdin.unwrap().write_all(PANGRAM.as_bytes()) {
        Ok(_) => println!("send to wc"),
        Err(why) => panic!("could not write to wc: {}", why),
    }

    let mut s = String::new();
    match process.stdout.unwrap().read_to_string(&mut s) {
        Ok(res) => println!("wc responded with: {}", res),
        Err(why) => panic!("coudl not read: {}", why),
    }
}

pub fn wait() {
    let mut child = Command::new("sleep").arg("5").spawn().unwrap();
    let _result = child.wait().unwrap();
    println!("readched end");
}

fn cat(path: &Path) -> io::Result<String> {
    let mut f = File::open(path)?;
    let mut s = String::new();

    f.read_to_string(&mut s)?;
    Ok(s)
}

fn echo(s: &str, path: &Path) -> io::Result<()> {
    let mut f = File::create(path)?;
    f.write_all(s.as_bytes())
}

fn touch(path: &Path) -> io::Result<()> {
    match OpenOptions::new().create(true).write(true).open(path) {
        Ok(_) => Ok(()),
        Err(e) => Err(e),
    }
}

pub fn filesystem() {}

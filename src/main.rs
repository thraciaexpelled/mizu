// main.rs - main entry code for `mizu`
// * this file is a part of `mizu`
// * `mizu` is MIT licensed by the authors of this software

use clap::{Parser, Subcommand};
use log::LevelFilter;
use log::set_max_level;

mod args;
mod terminal;

#[derive(Clone, Subcommand)]
enum Commands {
    /// Create a new project in the current working directory
    New {
        /// Name of the project
        name: String,

        /// Type of project
        #[arg(long)]
        proj_type: String,

        /// License to use with the project (default: MIT)
        #[arg(short, long)]
        license: Option<String>,
    },
}

/// Manage, Create or Use libraries written in C
#[derive(Parser)]
#[command(name = "mizu", author = "thraciaexpelled", version, about, long_about = None)]
struct Cli {
    /// Print debug and hidden messages
    #[arg(short, long)]
    verbose: bool,

    #[clap(subcommand)]
    command: Commands,
}

fn main() {
    colog::init();
    set_max_level(LevelFilter::Warn);
    let cli = Cli::parse();

    if cli.verbose {
        set_max_level(LevelFilter::Info);
    }

    match &cli.command {
        Commands::New { name, proj_type, license } => {
            // println!("Project Name:     {}", name);
            // println!("Project Type:     {}", proj_type);
            // println!("Project License:  {:?}", license);
            
            let j1 = args::SubcommandNew::initialize_implementation(
                name.to_string(), proj_type.to_string(), license.clone().unwrap_or_else(|| "MIT".to_string()),
            );
            j1.start();
        }
    }
}

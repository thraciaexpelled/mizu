// main.rs - main entry code for `mizu`
// * this file is a part of `mizu`
// * `mizu` is MIT licensed by the authors of this software

use clap::{Parser, Subcommand as ClapSubcommand};
use log::LevelFilter;
use log::set_max_level;

mod args;
mod project;
mod terminal;
mod mizu_subcommands;
mod subcommands;

pub use mizu_subcommands::MizuSubcommands;

#[derive(Clone, ClapSubcommand)]
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

    /// Initialize a new project in an existing directory
    Init {
        /// (Optional) Name of the project, omitting this will give the project name the same name as that of the CWD
        name: Option<String>,

        /// Type of project
        #[arg(long)]
        proj_type: String,

        /// License to use with the project (default: MIT)
        #[arg(short, long)]
        license: Option<String>,
    }
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

    let subcommand: Box<dyn MizuSubcommands> = match &cli.command {
        Commands::New { name, proj_type, license } => {
            Box::new(subcommands::new::SubcommandNew::initialize_implementation(
                name.to_string(),
                proj_type.to_string(),
                license.clone().unwrap_or_else(|| "MIT".to_string()),
            ))
        }
        Commands::Init { name, proj_type, license } => {
            let project_name = if let Some(n) = name {
                n.clone()
            } else {
                std::env::current_dir()
                    .expect("Failed to get current directory")
                    .file_name()
                    .expect("Failed to get directory name")
                    .to_string_lossy()
                    .into_owned()
            };

            Box::new(subcommands::init::SubcommandInit::initialize_implementation(
                project_name,
                proj_type.to_string(),
                license.clone().unwrap_or_else(|| "MIT".to_string()),
            ))
        }
    };

    subcommand.run();
}

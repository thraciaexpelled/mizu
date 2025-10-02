use log;
use serde::Serialize;
use std::fs;
use std::io::ErrorKind;
use std::path::Path;
use crate::terminal::cli_basic;
use crate::project::project::{self, FileStat};


// New: manage functionality for the subcommand `new`
// NOTE: `license` being not the same as on the `Command` struct on
//        main.rs has made the code really messy... refactor.
#[derive(Serialize)]
pub struct SubcommandNew {
	name: String,
	proj_type: String,
	license: String,
}

use crate::MizuSubcommands;

impl MizuSubcommands for SubcommandNew {
    fn run(&self) {
        log::info!("Creating project '{}'", self.name);

        match self.ensure_sanity() {
            Ok(FileStat::Directory) => {
                log::warn!("Project '{}' already exists in the current working directory", self.name);
                return;
            },
            Ok(FileStat::Exists) => {
                cli_basic::alert::creation(self.name.clone(), true);
                assert_eq!(project::mkdir(self.name.clone()), 0, "should not be erroneous");
            }
            Err(e) => {
                match e.kind() {
                    ErrorKind::NotFound => {
                        cli_basic::alert::creation(self.name.clone(), true);
                        assert_eq!(project::mkdir(self.name.clone()), 0, "should not be erroneous")
                    }
                    _ => log::trace!("{}", e),
                }
            },
            Ok(FileStat::NonExistent) | Ok(FileStat::Empty) => todo!(),
        }

        match project::create_config_file(&self.name, self) {
            Ok(conf_path) => {
                cli_basic::alert::creation(conf_path, false);
            }
            Err(e) => log::error!("{}", e),
        }

        cli_basic::normal::project_created(self.proj_type.clone(), self.name.clone());
    }
}

impl SubcommandNew {
    pub fn initialize_implementation(name: String, proj_type: String, license: String) -> SubcommandNew {
        SubcommandNew {
            name,
            proj_type,
            license
        }
    }

    pub fn ensure_sanity(&self) -> Result<FileStat, std::io::Error> {
        let binding = project::get_dir_create_filepath(self.name.clone());
        let project_path = Path::new(&binding);
        log::info!("Checking if {:?} exists...", project_path);

        let result = match fs::metadata(project_path) {
            Ok(meta) => {
                if meta.is_dir() {
                    log::info!("{:?} exists and is a directory.", project_path);
                    // TODO: check if that is our project or not
                    Ok(FileStat::Directory)
                } else {
                    log::warn!("{:?} exists but not as a directory. Need to create '{:?}'...", project_path, project_path);
                    Ok(FileStat::Exists)
                }
            }
            Err(e) => {
                // don't log anything unless if it's not a NotFound error
                match e.kind() {
                    ErrorKind::NotFound => Err(e),
                    _ => {
                        log::error!("Failed to check {:?}", project_path);
                        log::error!("Error: {:?}", e);
                        Err(e)
                    }
                }
            }
        };

        log::info!("All checks passed!");
        result
    }
}

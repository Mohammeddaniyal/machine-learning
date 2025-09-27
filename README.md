# Daniyal Machine Learning Framework (DMLFW)

## Introduction

Daniyal Machine Learning Framework (DMLFW) is a personal educational project to explore machine learning by building core algorithms and tools **from scratch in C**. It aims to teach and experiment with data preprocessing, training, prediction, and evaluation techniques.

***

## Installation

### Prerequisites

* GCC or any C compiler  
* Bash shell  

### Steps

1. Clone the repository:

   ```bash
   git clone https://github.com/Mohammedaniyal/machine-learning.git
   cd machine-learning
   ```

2. Build the core library:

   ```bash
   cd ml-framework/o_files
   sh create_lib.sh
   ```

   This compiles source files and generates `libml-framework.a` in `ml-framework/lib/`.

***

## Usage

* Prebuilt example programs are in the [`ml-examples/`](ml-examples/) folder. Explore, compile, and run to understand framework usage.  
* Command-line preprocessing tools are in [`tools/`](tools/), with executables built via `src/cmp.sh`. You may install these tools in a system path (e.g., `/usr/bin`) for ease of use or run them from their build folders.  
* You can build your own C programs linking against the library:

  ```bash
  gcc myprogram.c -I ./ml-framework/include -L ./ml-framework/lib -lml-framework -lm -o myprogram
  ```


* For detailed tutorials exploring all stages of workflow:  
  - Use [`ml-examples/regression/linear/TUTORIAL_PREPROCESSED.md`](ml-examples/regression/linear/TUTORIAL_PREPROCESSED.md) for running example programs with preprocessed datasets.  
  - Use [`ml-examples/regression/linear/TUTORIAL_RAW.md`](ml-examples/regression/linear/TUTORIAL_RAW.md) for step-by-step guidance on preprocessing raw datasets with the provided tools.  
  - Use [`tools/TUTORIAL.md`](tools/TUTORIAL.md) for comprehensive instructions on using the command-line preprocessing utilities.

***

## Project Structure

* `ml-framework/` – Core framework source code, headers, and build scripts.  
* `ml-examples/` – Sample programs demonstrating key ML algorithms and workflows, with `preprocessed/` and `test/` subfolders for datasets.  
* `tools/` – Command-line utilities for data preprocessing and encoding, with source and build folders.  
* `docs/` – Documentation files and generated Doxygen output.

***

## Learn More

* Explore comprehensive API references, detailed usage examples, and generated documentation in the [Doxygen Documentation](https://Mohammeddaniyal.github.io/machine-learning).  
* Browse additional practical code samples and supporting materials in the [`ml-examples/`](ml-examples/) folder to deepen your understanding and experimentation.  
* For broader context on framework design and implementation, consult the documentation files inside the [`docs/`](docs/) folder.

***
## Platform Support

* Built and tested on Linux with `pthread` and POSIX APIs.  
* Windows users should build under WSL or a compatible Linux environment.

***

## Contact

Maintainer: Mohammed Daniyal  
Email: [mohammeddaniyal453@gmail.com](mailto:mohammeddaniyal453@gmail.com)  
LinkedIn: [mohammeddaniyalali](https://www.linkedin.com/in/mohammeddaniyalali)  

***

Thank you for exploring DMLFW!  
This project is a work-in-progress learning journey—happy experimenting!

***


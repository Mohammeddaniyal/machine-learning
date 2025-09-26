# Daniyal Machine Learning Framework (DMLFW)


## About This Project

This project is part of my personal learning journey into **machine learning**. I wanted to go beyond just reading theory and actually **build things from scratch in C** to understand how ML works under the hood.  

The **Daniyal Machine Learning Framework (DMLFW)** is not meant to compete with big libraries like TensorFlow or PyTorch. Instead, it’s more like my **journal + toolkit**, where I try to implement basic data structures, math operations, and algorithms that form the backbone of ML.  

I’m improving it step by step as I learn more, so you can think of it as a work-in-progress learning playground.

---

## What’s Inside?

Right now, DMLFW includes:

- **Core Data Structures**  
  Vectors, matrices, and lists (supporting doubles and strings).  

- **Math & Stats Utilities**  
  Basic operations like addition, scaling, dot products, and transformations that are common in ML.  

- **Machine Learning Algorithms**  
  Linear regression with **batch, stochastic, and mini-batch gradient descent** implementations. More will be added as I learn them.  

- **Data Preprocessing**  
  Encoding categorical features, scaling, and normalization utilities.  

- **Model Evaluation**  
  Metrics like **R² score** and accuracy checks.  

- **Error Handling & Utilities**  
  A centralized system for error messages, plus some file and string utilities.  

The focus here is **clarity and education**, not speed or optimization.  

---

## 🔧 Installation & Setup

Here’s how you can get the framework running on your system:

###  Get the framework
Clone the repo from GitHub and move into the main folder:
```
git clone https://github.com/Mohammeddaniyal/machine-learning.git
cd machine-learning
```


### Build the core library
Go into the build script folder and compile:
```
cd ml-framework/o_files
sh create_lib.sh
```
If everything works, you’ll get:
```
ml-framework/lib/libml-framework.a
```


###  Compile your own program
Write your C program and include framework headers, for example:
```
#include <dmlfw_matrix.h>
// or use the umbrella header that includes everything
#include <dmlfw.h>
```
Then compile (adjust paths as needed):
```
gcc my_program.c -I ../include -L ../lib -lml-framework -lm -o my_program
```


###  Build examples or tools (optional)
Examples are inside:
```
ml-examples/regression/ (linear regression)
```
Tools are inside:
```
tools/src
```
Compile them the same way as your own program, for example:
```
gcc batch_gd.c -I ../ml-framework/include -L ../ml-framework/lib -lml-framework -lm -o batch_gd
```

###  Test the setup
Run a tool (from `tools/build`):
```
./encode_one_hot ../../tools/testcases/insurance.csv output.csv sex smoker region
```
Or run an ML example (like batch gradient descent):
```
./ml-examples/regression/linear/src/batch_gd
```

If you see proper output, congrats — the framework is working!

---

### Notes
- Always add **`-lm`** at the end of your gcc command (for math library).  
- Use **`./`** when running binaries from the current folder.  
- On **Windows**, it’s easiest to use **Git Bash** or **WSL** for building and running.  
```

---


## 📂 Project Structure

The repo is organized into:

- **Core Framework:** vectors, matrices, ML algorithms, encoders, scalers.
- **Tools:** simple command-line programs for preprocessing datasets.
- **Examples:** demo programs for regression and gradient descent.

I’m also generating **Doxygen documentation** (link coming soon), which explains the API in detail.

---

## 🚀 Examples You Can Try

Inside `ml-examples`, you’ll find demos like:

- Linear regression using **batch gradient descent**
- Linear regression using **stochastic/mini-batch gradient descent**
- Preprocessing and encoding datasets

Running these helped me a lot in actually “seeing” the math in action.

---

## Contributions

This project is mainly for my learning, but if anyone wants to try it out or suggest improvements, feel free.  

If you’d like to contribute, please open an issue or message me with your idea.  
The main goal is to **keep it simple and educational**.

---

## Acknowledgments

A big thank you to my mentor **Prafull Kelkar Sir** for guiding me and encouraging me to dig deeper into ML instead of just staying at the surface level.

---

##  Contact Me

If you want to connect, give feedback, or just chat about ML:

- 📧 Email: [mohammeddaniyal453@gmail.com](mailto:mohammeddaniyal453@gmail.com)
- 📱 Phone: +91 8770642372
- 💼 LinkedIn: [mohammeddaniyalali](https://www.linkedin.com/in/mohammeddaniyalali)

---

✨ Thanks for checking this out. This is still the beginning of my ML journey, and DMLFW is the notebook where I write code instead of just notes. Hopefully, it grows into something more useful over time.
```

Would you like me to also create a shorter "Quick Start" snippet (just build + run example) for people who might not want to read the whole README?

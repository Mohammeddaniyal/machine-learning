# Daniyal ML Framework - ML Examples Tutorial

## Overview

This tutorial guides you step-by-step through compiling and running machine learning example programs using **preprocessed datasets**.  
You will learn how to run various gradient descent algorithms on sample datasets like `IceCreamSales` and `trends`, how to adjust training parameters, and evaluate models.

---

## 1. Folder Structure

- `src/` — source code for ML example programs  
- `build/` — compiled executables (after compilation)  
- `test/` — raw datasets for preprocessing and experimentation  
- `preprocessed/` — preprocessed datasets ready-to-use, organized by dataset name  

Example preprocessed data folder for IceCreamSales:

```
ml-examples/regression/linear/preprocessed/IceCreamSales/
├── IceCreamSales.csv
├── IceCreamSales_training_examples.csv
├── IceCreamSales_testing_examples.csv
├── example-1-model.csv
├── example-2-model.csv
├── example-3-model.csv
├── cost.csv
├── line.csv
```

Example folder for Trends dataset:

```
ml-examples/regression/linear/preprocessed/trends/
├── poly_trends_test.csv
├── scaled_poly_trends_test.csv
├── poly_trends_train.csv
├── scaled_poly_trends_train.csv
├── trends.csv
├── trends_train.csv
├── trends_test.csv
```

---

## 2. Compilation

### Compile a specific example program

Change to the `src/` folder, make the compilation script executable, then compile a single program:

```
cd ml-examples/regression/linear/src
chmod +x cmp.sh
./cmp.sh batch_gd
```

Or compile polynomial batch gradient descent for trends:

```
./cmp.sh polynomial_batch_gd
```

Executables are generated in the `build/` folder at the same level as `src/`.

---

## 3. Running Examples with Preprocessed Data

### Important: Do **not** interrupt the training process early (e.g., with Ctrl+C).

Training must complete all iterations to save the model correctly. Interrupting prematurely will lose the progress and no model file will be saved. To change the number of training iterations, update the `ITERATION_NUMBER` macro in the source code and recompile.

---

### 3.1 IceCreamSales Examples

#### Mini-Batch Gradient Descent

Run mini-batch gradient descent on IceCreamSales dataset:

```
cd ml-examples/regression/linear/preprocessed/IceCreamSales
../../build/mini_batch_gd
```

This will run all training iterations and upon completion generate the model file `example-3-model.csv`.

#### Evaluate Model Accuracy

After training finishes, evaluate the model:

```
../../build/gd_test_model example-3-model.csv
```

Example output:

```
Accuracy score is : 0.965031
```

---

### 3.2 Stochastic Gradient Descent

Run stochastic gradient descent:

```
../../build/stochastic_gd
```

Model saved as `example-2-model.csv` after all iterations finish.

Evaluate accuracy:

```
../../build/gd_test_model example-2-model.csv
```

---

### 3.3 Batch Gradient Descent

Run batch gradient descent (may take more iterations):

```
../../build/batch_gd
```

Model saved as `example-1-model.csv` upon completion.

Evaluate accuracy:

```
../../build/gd_test_model example-1-model.csv
```

---

### 3.4 Trends Dataset: Polynomial Batch Gradient Descent

Run polynomial batch GD for trend prediction:

```
cd ml-examples/regression/linear/preprocessed/trends
../../build/polynomial_batch_gd
```

Wait for all iterations to complete. Model saved as `example-4-model.csv`.

Evaluate accuracy:

```
../../build/gd_test_model example-4-model.csv
```

Example output:

```
Accuracy score is : 0.943412
```

---

## 4. Adjusting Training Length

To change the number of training iterations (for any example), modify the source code macro `ITERATION_NUMBER` in the relevant `.c` file inside `src/`. Recompile the program using `./cmp.sh <program_name>` after changes.

---

## 5. Summary

- Compile example programs individually for quick iteration using `./cmp.sh <program_name>`.  
- Preprocessed datasets in `preprocessed/` folders allow immediate running without further data preparation.  
- Training programs must run until all iterations complete to save model files. Interrupting early loses progress.  
- Evaluate trained models with `gd_test_model` executable and model CSV files.

---


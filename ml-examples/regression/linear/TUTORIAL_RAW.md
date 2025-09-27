# Daniyal ML Framework - Raw Data Preprocessing and Training Tutorial

## Overview

This tutorial guides you through preprocessing **raw datasets** and training models using ML example programs.  
We cover preprocessing, scaling, train/test split, model training, and evaluation with raw `IceCreamSales` and `trends` datasets.

***

## Important Note About Preprocessing Tools

The examples in this tutorial assume you have copied or installed the preprocessing tools (such as `create_test_train_dataset`, `polynomial_dataset_generator`, `scale_dataset`, etc.) into a directory included in your system's executable `PATH`, for example `/usr/bin`. This allows you to run these tools simply by their names without specifying full or relative paths.

If you have not done this, you need to run the tools by specifying their full or relative paths, for example:

```bash
../../tools/build/create_test_train_dataset ...
```

***

## 1. IceCreamSales Dataset Example

### Step 1: Create Train/Test Split

From the `test/IceCreamSales` folder:

```bash
create_test_train_dataset IceCreamSales.csv IceCreamSales_testing_examples.csv IceCreamSales_training_examples.csv 20
```

Generates training and testing CSV files with 20% test data.

***

### Step 2: Run Mini-Batch Gradient Descent Training

```bash
../../build/mini_batch_gd
```

This will run all iterations configured in source and save the model as `example-3-model.csv`. **Do not interrupt early.**

***

### Step 3: Evaluate the Model

```bash
../../build/gd_test_model example-3-model.csv
```

Expected output:

```
Accuracy score is : 0.965031
```

***

### Step 4: Running Other Algorithms

Stochastic Gradient Descent:

```bash
../../build/stochastic_gd
```

Evaluate:

```bash
../../build/gd_test_model example-2-model.csv
```

Batch Gradient Descent:

```bash
../../build/batch_gd
```

Evaluate:

```bash
../../build/gd_test_model example-1-model.csv
```

***

## 2. Trends Dataset Example

### Step 1: Generate Synthetic Dataset

Using dataset generator:

```bash
curve_dataset_generator trends.csv 10 120
```

***

### Step 2: Create Train/Test Split

```bash
create_test_train_dataset trends.csv trends_test.csv trends_train.csv 20
```

***

### Step 3: Generate Polynomial Features

For test and train sets:

```bash
polynomial_dataset_generator trends_test.csv poly_trends_test.csv 7
polynomial_dataset_generator trends_train.csv poly_trends_train.csv 7
```

***

### Step 4: Scale Datasets

Scale training data (using z-score):

```bash
scale_dataset poly_trends_train.csv scaled_poly_trends_train.csv z-score z_score.csv 0 1 2 3 4 5 6
```

Scale test data using training parameters:

```bash
scale_using_parameters_file poly_trends_test.csv scaled_poly_trends_test.csv z-score z_score.csv
```

***

### Step 5: Train Polynomial Batch Gradient Descent

```bash
../../build/polynomial_batch_gd
```

Wait for training completion to generate `example-4-model.csv`.

***

### Step 6: Evaluate the Model

```bash
../../build/gd_test_model example-4-model.csv
```

Expected output:

```
Accuracy score is : 0.943412
```

***

## 3. Summary

- Use the provided tools to split data, generate polynomial features, and scale datasets step-by-step.  
- Always let training finish the configured iterations to save the model. Interrupting early loses progress.  
- Evaluate models using `gd_test_model` on generated model CSV files.  
- Repeat for other datasets by following analogous preprocessing steps.


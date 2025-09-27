# Daniyal ML Framework - Tools Tutorial

## Overview

This tutorial explains how to compile and use all preprocessing tools in the `tools/` folder. It provides detailed usage examples on sample datasets.

***

## 1. Compilation

Compile all tools or a specific tool using the `cmp.sh` script inside the `src/` folder.

### Compile all tools:

```bash
cd src
chmod +x cmp.sh
./cmp.sh
```

This compiles all tools individually and places the executables in the root-level `build/` folder.

### Compile a single tool (e.g., column_cutter):

```bash
./cmp.sh column_cutter
```

Executes compilation only for the specified tool, producing its binary in `build/`.

***

## 2. Dataset Files

Example datasets are provided in the `testcases/` folder to test and experiment with:

- `insurance.csv`  
- `IceCreamSales.csv`  
- `pet_adoption_data.csv`  
- `pet_adopt.csv` (pet adoption data with `PetID` removed)

***

## 3. Running Tools

When running tools from inside the `testcases/` folder, use relative paths to call executables in `build/`. For example:

```bash
cd testcases
../build/binary_encode insurance.csv insurance_binary.csv sex smoker region
```

This runs the `binary_encode` tool on the `insurance.csv` dataset.

***

### Usage examples for all tools:

#### binary_encode

```bash
../build/binary_encode insurance.csv insurance_binary.csv sex smoker region
```

***

#### column_cutter

```bash
../build/column_cutter pet_adoption_data.csv pet_adopt.csv PetID
```

***

#### create_test_train_dataset

```bash
../build/create_test_train_dataset insurance.csv test.csv train.csv 20
```

***

#### curve_dataset_generator

```bash
../build/curve_dataset_generator generated_curve.csv 0 100
```

***

#### encode_binary

```bash
../build/encode_binary insurance.csv insurance_encoded.csv sex smoker region
```

***

#### encode_one_hot

```bash
../build/encode_one_hot insurance.csv insurance_one_hot.csv sex smoker region
```

***

#### min_max_scaler

```bash
../build/min_max_scaler insurance_one_hot.csv insurance_minmax.csv 0 6 min_max_params.csv min-max
```

***

#### min_max_test_data_scaler

```bash
../build/min_max_test_data_scaler test_data.csv scaled_test_data.csv 0 6 minmax_params.csv min-max
```

***

#### polynomial_dataset_generator

```bash
../build/polynomial_dataset_generator IceCreamSales.csv poly_icecream.csv 3
```

***

#### r2score

```bash
../build/r2score prediction_results.csv
```

***

#### reverse_min_max_scaler

```bash
../build/reverse_min_max_scaler predicted_scaled.csv prediction_original.csv minmax_params.csv
```

***

#### scale_dataset

```bash
../build/scale_dataset insurance_onehot.csv scaled_insurance.csv min-max minmax_params.csv age bmi children charges
```

***

#### scale_using_parameters_file

```bash
../build/scale_using_parameters_file insurance_onehot.csv scaled_insurance.csv min-max minmax_params.csv
```

***

## 4. Summary

- Compile using `chmod +x cmp.sh` and `./cmp.sh` in `src/`.  
- Use relative paths when running tools from `testcases/` folder to access `../build/<tool>`.  
- For scaling, use algorithm names `min-max` or `z-score`.

- Run any tool with no arguments to see its usage message.

***

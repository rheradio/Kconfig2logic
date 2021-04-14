# Kconfig2logic: A Kconfig-to-Propositional Logic Translation

**Software artifact accompanying the paper "A Validating Approach to Translating Kconfig-Based Systems to Propositional Logic" submitted to the Empirical Software Engineering journal**

## Artifact

The artifact has been tested on Linux and macOS.

It requires recent versions of flex, bison, gperf and GNU xargs. You can get them in Linux by typing:
```
sudo apt install flex bison gperf libgmp-dev libfl-dev
```

Compilation instructions:

```
cd translations/KconfigReader
unzip freetz.zip
cd ../../code
make
cd ..

```

Use scripts at will. Examples:

```
./createVarorderings.sh (to translate the systems using Kconfig2Logic)
./validateTranslation.sh <system name> <number of configs> to validate random configurations of Kconfig2Logic translation
for instance ./validateTranslation.sh axtls
./validaTranslationConcurrent.sh <system name> <number of configs> <number of threads> to validate random configurations of Kconfig2Logic translation using several threads
for instance, ./validateTranslationConcurrent.sh busybox 100 5
./validateKconfigReader.sh <system name> <number of configs> to validate KconfigReader translation
for instance, ./scripts/validateKconfigReader.sh axtls 100.

Say instance 62 failed because of too many solutions. You can see what happened typing:
./code/bin/looseVars systems/axtls/axtls.62
Variable    CONFIG_X509_MAX_CA_CERTS_EQ_0 can be true or false 
Variable    CONFIG_X509_MAX_CA_CERTS_EQ_1 can be true or false 
Variable  CONFIG_X509_MAX_CA_CERTS_EQ_150 can be true or false 
Variable        MUL_KARATSUBA_THRESH_EQ_0 can be true or false 
Variable        MUL_KARATSUBA_THRESH_EQ_1 can be true or false 
Variable       MUL_KARATSUBA_THRESH_EQ_20 can be true or false ```
```

You can also use ./scripts/cleanResults.sh to delete the wrong instances.

The programs in the bin directory can also be used. Type the name to see the documentation. BDD file names must not include the extension (.dddmp)


## Acknowledgements

This software artifact has been funded by the Spanish Ministry of Science and Innovation, under grant with reference DPI2016-77677-P.

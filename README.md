# Kconfig2logic: A Kconfig-to-Propositional Logic Translation

*Software artifact accompanying the paper "Validating Variability Modeling Translations to Logic Applied to Kconfig" submitted to the Empirical Software Engineering journal*

Requires recent versions of flex, bison, gperf and GNU xargs. Tested on Linux and macOS.
Compilation instructions:
cd code
make
enter root password to install CUDD libraries.
cd ..

export KCONF=`pwd`
use scripts at will. Examples:
./scripts/createVarorderings.sh (to translate the systems using Kconfig2Logic)
./scripts/validateTranslation.sh <system name> <number of configs> to validate random configurations of Kconfig2Logic translation
./scripts/validaTranslationConcurrent.sh <system name> <number of configs> <number of threads> to validate random configurations of Kconfig2Logic translation using several threads
./scripts/validateKconfigReader.sh <system name> <number of configs> to validate KconfigReader translation

The programs in the bin directory can also be used. Type the name to see the documentation. BDD file names must not include the extension (.dddmp)

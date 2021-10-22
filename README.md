# Foldase-
This the repository for the IGEM DryLab Team

Hi everyone we are the UCDavis 2021 iGEM team and this is out github repository for our RNA folding package, Foldase! 
Foldase is a folding software based on the Vienna RNA Package that inserst an IRE into a desired 5'UTR of a seondary sequence at every postion to siolate the bets possible location in the 5' region. This is done in cooperation with Dr Radecki's Fold-Fastalike (https://github.com/peradecki/fold-fastalike) which is required for the second part of the 3 main pipeline code.

First steps are to download the Python version ViennaRNA folding package. Although there is a perl version of the package we did not support this version to streamline the process for those more well equipped with Python. AFter dowloading Vienna, you will want to set up Fold-Fastalike which was liked above, the download requirements and dependabekls are all outlined on the page.

From here after downloading and running the pythong scripts in ther terminal the user is propmpet for 3 inputs each of which being the files for the respetice IRE, 5' and, rest of the sequence. This will generate a fasta file contaning all of the sequences that are then passed into Fold-Fastalike which will generate the global kissing graph and base pare distributions.

The final step is comparing each of the shifted basepair probabilities with that of the IRE which is done manually to sort through the desired best candiate

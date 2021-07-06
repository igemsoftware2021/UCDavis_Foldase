import RNA


# simple script to test the free energy of each sequence

#Function to calculate both SEC and MFE
def Calculate_SEC_MFE(sequences):
    # RNA Sequence dictory with sequencence and free energy
    temp_dict = sequences

    # compute minimum free energy (MFE) and corresponding structure for each of the sequences stored in the dictionary
    for sequence in temp_dict:
        mfe = RNA.fold(sequence)
        temp_dict[sequence] = mfe

    # loop through the dictionary and print sequence with the secondary free structure and free energy
    for sequence in temp_dict:
        print("Sequence: ", sequence + ", " + "Secondary structure and Free Energy: " + str(temp_dict[sequence]), "\n")


# IRE that needs to be inserted
IRE = "GCUCGUUCCCCACCCCGGCCGGCCGCCC"

# 5 Prime reagion of the Sequence
FPRIME = "ACATTTGCTTCTGACACAACTGTGTTCACTAGCAACCTCAAACAGACACC"

# dictionary which contains each of the sequences
mydict = {}


# Function that generates the sequences
def Generate_Sequences(Dict):
    # loops through the dictionary and inserts the IRE into each position of the 5PRIME region
    for i in range(len(FPRIME)+1):
        test = FPRIME[:i] + IRE + FPRIME[i:]
        Dict[test] = 0

  

#Runs the functions
Generate_Sequences(mydict)
Calculate_SEC_MFE(mydict)

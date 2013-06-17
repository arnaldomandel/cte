
NAME
	G4L.pl 


DESCRIPTION

	Context tree selection using the Smallest Maximizer Criterion (SMC). 

AUTHORS 

	Antonio Galves, Charlotte Galves, Jesus E. Garcia, Nancy L. Garcia, Florencia Leonardi.


REQUIREMENTS 

	MAC OS X. Tested on MAC OS X version 10.6.8. Should work in OS X Leopard.
	
	R must be installed on the computer. The perl script call R-project to perform part of the Bootstrap procedure. 

	A text file named abcde.txt containing the alphabet should be on the same directory. 


USAGE

	perl G4L.pl name datafile deep nr_Ress epsilon min_c max_c 4_is_renov_point prop_Ress_1 prop_Ress_2 test_level 


ARGUMENTS
	
	name	: suffix to the file containing the results.

	datafile: ASCII file containing the sample.

	deep	: maximum possible deep for the fitted context tree.

	nr_Ress	: number of resamples in the bootstrap procedure. See Reference.

	epsilon	: tolerance used when estimating the set of champion trees. See Reference. 

	min_c	: minimum value for the constant c. See Reference.

	max_c	: maximum value for the constant c. See Reference.

	4_is_renov_point: this argument has to be set to 0, except in the linguistic application in which the letter 4 of the alphabet is a renovation point. See Reference.

	prop_Ress_1	: proportion of the size of the sample corresponding to the size of the smaller resample. We used prop_Ress_1 = 0.3, see Reference.

	prop_Ress_2	: proportion of the size of the sample corresponding to the size of the larger resample.  We used prop_Ress_2 = 0.9, see Reference.
	
	test_level	: alpha significance level for the t-test.  We used test_level = 0.01, see Reference.


DETAILS:

The datafile should be an ASCII file with the sequence of letters without any separation. As an example see the file "folha.txt".

The file "dataset.AUX.name.txt" will contain a log of the procedure including the set of Champion Trees, the bootstrap procedure ant the chosen tree.


REFERENCE

Context tree selection and linguistic rhythm retrieval from written texts. To appear in Annals of Applied Statistics.
Can be downloaded from ArXiv: 0902.3619v3, 2011. 


EXAMPLE

From the linguistic application. 


To select the context tree corresponding to European Portuguese using the SMC:

an ASCII file named "abcde.txt" containing the sequence 01234 should be on the same directory with the files "G4L.pl" and "publico.txt";

perl G4L.pl EP publico.txt  4 200 0.01 0.1 400 1 .3 .9 .01


To select the context tree corresponding to Brazilian Portuguese using the SMC:

an ASCII file named "abcde.txt" containing the sequence 01234 should be on the same directory with the files "G4L.pl" and "folha.txt";

perl G4L.pl BP folha.txt  4 200 0.01 0.1 400 1 .3 .9 .01




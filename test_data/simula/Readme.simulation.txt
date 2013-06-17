 
NAME:

      simulation.pl

DESCRIPTION 

	Program used to perform the simulation study appearing in Annals of Applied Statistics:

	Context tree selection and linguistic rhythm retrieval from written texts.
 
	Can be downloaded from ArXiv: 0902.3619v3, 2011.

 AUTHORS 

	Antonio Galves, Charlotte Galves, Jesus E. Garcia, Nancy L. Garcia, Florencia Leonardi.


 REQUIREMENTS 

   - MAC OS X. Tested on MAC OS X version 10.6.8. Should work in OS X Leopard.
   
   - R must be installed on the computer. The perl script call R-project to perform part of the Bootstrap procedure. 


 USAGE

     perl simulation.pl name sample_size nr_sim model

 ARGUMENTS
    
   name       : suffix to the file containing the results.

   sample_size: size of each sequence

   nr_sim     : number of sequences to be simulated.

   model      : 1 for model 1 and 2 for model 2. 

 DETAILS:

   The file "Results.name.txt" will contain a log of the procedure and the final results.

 REFERENCE: Context tree selection and linguistic rhythm retrieval from written texts. ArXiv: 0902.3619v3, 2011. 

 EXAMPLE: 

  To apply the SMC procedure to 100 simulated samples of size 10000 of model 1.

            perl simulation.pl sim1 10000 100 1

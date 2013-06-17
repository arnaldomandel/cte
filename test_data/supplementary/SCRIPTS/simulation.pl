#!/usr/local/bin/perl
#
# 
#NAME:
#
#      simulation.pl
#
#DESCRIPTION 
#
#	Program used to perform the simulation study appearing on:
#
#	Context tree selection and linguistic rhythm retrieval from written texts. ArXiv: 0902.3619v3, 2011.
#
# AUTHORS 
#
#	Antonio Galves, Charlotte Galves, Jesus E. Garcia, Nancy L. Garcia, Florencia Leonardi.
#
#
# REQUIREMENTS 
#
#   - MAC OS X. Tested on MAC OS X version 10.6.8. Should work in OS X Leopard.
#
#   - R must be installed on the computer. The perl script call R-project to perform part of the Bootstrap procedure. 
#
#
# USAGE
#
#     perl simulation.pl name sample_size nr_sim model
#
# ARGUMENTS
#
#   name       : suffix to the file containing the results.
#
#   sample_size: size of each sequence
#
#   nr_sim     : number of sequences to be simulated.
#
#   model      : 1 for model 1 and 2 for model 2. 
#
# DETAILS:
#
#   The file "Results.name.txt" will contain a log of the procedure and the final results.
#
# REFERENCE: Context tree selection and linguistic rhythm retrieval from written texts. ArXiv: 0902.3619v3, 2011. 
#
# EXAMPLE: 
#
#  To apply the SMC procedure to 100 simulated samples of size 10000 of model 1.
#
#            perl simulation.pl sim1 10000 100 1
#
#
$nome         = $ARGV[0];
$N            = $ARGV[1]; #SAMPLE SIZE
$Nsim         = $ARGV[2]; #Nr. simulations
$MOD	      = $ARGV[3];



$linea = "01";
$LA       = length($linea);
$alphabet = $linea;
@l_sep    = split( //, $linea );
print "Alphabet:@l_sep\n";
for ( $i = 0 ; $i < $LA ; $i++ ) { @NA[$i] = $i; }

$eps          = 0.00001;
$PBSS0        = 0.3;
$PBSS1        = 0.9;
$alpha		  = 0.01;
$Bstr         = 200;
$Dmax         = 6;
$BSS0=int($PBSS0*$N);
$BSS1=int($PBSS1*$N);
@BSSS[0]=$BSS0;
@BSSS[1]=$BSS1;
$LBSSS=2;

open( TREES, ">$MOD.TREES.$nome.$N.$Nsim.txt" );
close(TREES);
open( CyT, ">$MOD.CyTREES.$nome.$N.$Nsim.txt" );
close(CyT);
open( BIC, ">$MOD.BIC.$nome.$N.$Nsim.txt" );
close(BIC);
open(RESULTADO , ">$MOD.resultados.$nome.bs.lml.$N.$Nsim.txt");
close(RESULTADO );
open(NUESTRO , ">$MOD.ArboreEscolhida.$nome.bs.lml.$N.$Nsim.txt");
close(NUESTRO );
$Ncampeon=0;
$NBIC=0;
$NUESTRO=0;


for($ns=1;$ns<=$Nsim;$ns++){
#SIMULA
if ($MOD == 1){$data = &simMod1( $N );}
elsif ($MOD == 2){$data = &simMod2( $N );}

@split_data = split( /1/, $data );
$smatch = "?=1";
$nut = $data =~ s/($smatch)/$1/g;

for ( $iseq = 1 ; $iseq <= $nut ; $iseq++ ) {
    @split_data[$iseq] = '1' . @split_data[$iseq];
}





$BICtree = &CTM( $data, $Dmax, $LA, $l_sep, 0.5 );
print("\n==============================\nSample $ns,\nBIC tree= $BICtree\n");
if ($BICtree eq " 000 1 10 100"){$NBIC++;}
open( BIC, ">>$MOD.BIC.$nome.$N.$Nsim.txt" );
print(BIC "$ns, $BICtree\n");
close(BIC);

$Ntr=0;
$A=0;
$B=100;
$Atree = &CTM( $data, $Dmax, $LA, $l_sep, $A );
$Btree = &CTM( $data, $Dmax, $LA, $l_sep, $B );
@chtree[$Ntr]=$Atree;
open( TREES, ">>$MOD.TREES.$nome.$N.$Nsim.txt" );
open(RESULTADO , ">>$MOD.resultados.$nome.bs.lml.$N.$Nsim.txt");
open(NUESTRO , ">>$MOD.ArboreEscolhida.$nome.bs.lml.$N.$Nsim.txt");
print(NUESTRO "$ns \n");
open( CyT, ">>$MOD.CyTREES.$nome.$N.$Nsim.txt" );
print(CyT "$ns. $Ntr c=$A Tree[$A]=@chtree[$Ntr]\n");
print(TREES "@chtree[$Ntr] , ");
print(RESULTADO  "@chtree[$Ntr] ,");
print(NUESTRO  "$Ntr:@chtree[$Ntr] ,");

if ($chtree[$Ntr] eq " 000 1 10 100"){$Ncampeon++;}
$Ntr++;
while ($Atree ne $Btree){
$dif=$B-$A;	
while ($dif > $eps){
while ($Atree ne $Btree){
	$Bold=$B;
	$Boldtree=$Btree;
	$B=($A+$B)/2;
	$Btree = &CTM( $data, $Dmax, $LA, $l_sep, $B );
}
$A=$B;
$B=$Bold;
$Btree=$Boldtree;
$dif=$B-$A;
}

$A=$B;
$Atree=$Btree;
@chtree[$Ntr]=$Atree;
print(CyT "$ns. $Ntr c=$B Tree[$B]=@chtree[$Ntr]\n");
print(TREES "@chtree[$Ntr] , ");
print(RESULTADO  "@chtree[$Ntr] ,");
print(NUESTRO  "$Ntr:@chtree[$Ntr] ,");

if ($chtree[$Ntr] eq " 000 1 10 100"){$Ncampeon++;}
$Ntr++;
$B=100;
$Btree = &CTM( $data, $Dmax, $LA, $l_sep, $B );
}
print(CyT "\n");
print(TREES "\n");
print(RESULTADO  "\n");
print(NUESTRO  "\n");
close(NUESTRO);
close(RESULTADO );
close(CyT);
close(TREES);


############ Bootstrap ###########################

for($nbsss=0;$nbsss<$LBSSS;$nbsss++){
#	print("BS, size=$BSSS[$nbsss]\n");
if($nbsss==0){open( BS0, ">$MOD.$nome.bs.lml.$BSSS[$nbsss].$N.$Nsim.txt" );
print(BS0 "@cch\n");}
if($nbsss==1){open( BS1, ">$MOD.$nome.bs.lml.$BSSS[$nbsss].$N.$Nsim.txt" );
print(BS1 "@cch\n");}
}


	for($bi=1;$bi<=$Bstr ;$bi++){	
		$datacompl = &reamostra( $split_data, $nut,$BSSS[($LBSSS-1)] );
		if ($bi==2){
			open(MUESTRA,">$MOD.MUESTRA2.$nome.txt");
			print(MUESTRA "$datacompl\n");
			close(MUESTRA);
		}
		for($nbsss=0;$nbsss<$LBSSS;$nbsss++){
			$data=substr($datacompl,0,$BSSS[$nbsss]);
			@LMLT[0]=&LML( $data, $LA, $l_sep, $chtree[0] );
			#print"LMLT(0)=@LMLT[0]";
			for($nt=1;$nt<$Ntr;$nt++){
				@LMLT[$nt]=&LML( $data, $LA, $l_sep, $chtree[$nt] );
	#print"nt=$nt, LMLT($nt)=@LMLT[$nt]";
#				@dif[$nt]=(@LMLT[$nt-1]-@LMLT[$nt])/$BSSS[$nbsss];
			}
	if($nbsss==0)	{	print(BS0 "@LMLT\n");}
	if($nbsss==1)	{	print(BS1 "@LMLT\n");}

		}
	}
	if($nbsss==0)	{close(BS0);}
	if($nbsss==0)	{close(BS1);}

$SEQTR="";
for($nt=0;$nt<$Ntr;$nt++){
	$ntm1=($nt+1);
	$SEQTR=$SEQTR."chtree[$ntm1] <- \"$chtree[$nt]\"
";	
}

$comando="bs1<-read.csv(file=\"$MOD.$nome.bs.lml.$BSSS[0].$N.$Nsim.txt\",sep=\" \",header=FALSE)
bs2<-read.csv(file=\"$MOD.$nome.bs.lml.$BSSS[1].$N.$Nsim.txt\",sep=\" \",header=FALSE)
NN<-$Bstr 
BS<-c($BSSS[0],$BSSS[1])
Ntree<-$Ntr - 1

qlml3<-qlml2<-qlml1<-matrix(c(1:(Ntree*NN)),Ntree,NN)

chtree<-c(\" \")
$SEQTR

for (i in 1:Ntree){
	for(j in 1:NN){
		qlml1[i,j]<- (bs1[j,i] - bs1[j,i+1])/(BS[1]**0.9)
	}
}

for (i in 1:Ntree){
	for(j in 1:NN){
		qlml2[i,j]<- (bs2[j,i] - bs2[j,i+1])/(BS[2]**0.9)
	}
}



pdf(file=\"$MOD.resultados.$nome.bs.lml.$N.$Nsim.$ns.pdf\")
layout(t(matrix(c(1:8),4,2)))

for(i in 1:Ntree){
	boxplot(qlml1[i,],qlml2[i,])
}

alpha=$alpha
i<- Ntree+1
p.valor<-1
p.v<-0
max<-0

while((p.valor > alpha) && (i>1)){	
i<-i-1
p.valor<-t.test(qlml1[i,], qlml2[i,],paired = TRUE,alternative = \"greater\")\$p.value
print(c(i,p.valor));
SD1<-sd(qlml1[i,])
SD2<-sd(qlml2[i,])
write(c(i,p.valor,SD1,SD2),file=\"$MOD.resultados.$nome.bs.lml.txt\",append=TRUE)
}
I<-i+1
print(chtree[I]);
arv <- I
write(chtree[I],file=\"$MOD.ArboreEscolhida.$nome.bs.lml.txt\",append=TRUE)
write(chtree[I],file=\"$MOD.Ultima.ArvoreEscolhida.txt\",append=FALSE)
";
open( RES, ">$MOD.BS.r" );
print(RES "$comando\n");
close(RES);
$linea="R CMD BATCH $MOD.BS.r";

system $linea;

open( ULT, "<$MOD.Ultima.ArvoreEscolhida.txt" );
$linea = <ULT>;
chomp($linea);
if ($linea eq " 000 1 10 100"){$NUESTRO++;}
close(ULT);
print("SMC tree= $linea\n");

$delete = " rm $MOD.Ultima.ArvoreEscolhida.txt
rm $MOD.ArboreEscolhida.$nome.bs.lml.txt
rm $MOD.CyTREES.$nome.$N.$Nsim.txt
rm $MOD.$nome.bs.lml.$BSSS[0].$N.$Nsim.txt
rm $MOD.$nome.bs.lml.$BSSS[1].$N.$Nsim.txt
rm $MOD.resultados.$nome.bs.lml.$N.$Nsim.$ns.pdf
rm $MOD.resultados.$nome.bs.lml.txt
rm $MOD.ArboreEscolhida.$nome.bs.lml.$N.$Nsim.txt
rm $MOD.TREES.$nome.$N.$Nsim.txt
rm $MOD.resultados.$nome.bs.lml.$N.$Nsim.txt
rm $MOD.BS.r
rm $MOD.BS.r.Rout
rm $MOD.BIC.$nome.$N.$Nsim.txt
rm $MOD.MUESTRA2.$nome.txt
";
system $delete;

}
open( AUX, ">>Results.$nome.txt" );
print(AUX "\n \n       
Size = $N          
Nr. Sim = $Nsim       
Model = $MOD\n");		
print(AUX "$Nsim size $N sequences where simulated from model $MOD,\n"); 
print(AUX "the true tree was chosen by the BIC criterion $NBIC times,\n");
print(AUX "the true tree was chosen by the SMC procedure $NUESTRO times.\n");

print( "$Nsim size $N sequences where simulated from model $MOD,\n"); 
print( "the true tree was chosen by the BIC criterion $NBIC times,\n");
print( "the true tree was chosen by the SMC procedure $NUESTRO times.\n");

########################################################

sub reamostra {
    my ( $split_data, $nut, $N  )= @_;
    $data = "";

	$tamseq=0;
   while ($tamseq <= $N ){ 
	$r = int( rand($nut) );

        $data   = $data . @split_data[$r];
        $tamseq = length($data);
    }


    $data = substr( $data, 0, $N );

    return ($data);
}

sub simMod1 {
    my ($N)= @_;

    #1
    $p0d1 = 1;
    $p1d1 = 0;

    #10
    $p0d01 = 0.3;

    #000
    $p0d000 = 0.25;

    #100
    $p0d001 = 0.20;

    @X[1] = 1;
    @X[0] = 1;
    $CM   = $X[1];

    for ( $i = 2 ; $i <= ( $N + 1 ) ; $i++ ) {
        $U = rand(1);

        if ( @X[ $i - 1 ] == 1 ) { $X[$i] = 0 }
        else {
            if ( @X[ $i - 2 ] == 1 ) {
                if   ( $U > $p0d01 ) { $X[$i] = 1 }
                else                 { $X[$i] = 0 }
            }
            else {
                if ( @X[ $i - 3 ] == 1 ) {
                    if   ( $U > $p0d001 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }
                else {
                    if   ( $U > $p0d000 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }

            }

        }

        if ( $i > 0 ) { $CM = $CM . $X[$i]; }
    }

    while ( @X[ $i - 1 ] != 1 ) {
        $U = rand(1);

        if ( @X[ $i - 1 ] == 1 ) { $X[$i] = 0 }
        else {
            if ( @X[ $i - 2 ] == 1 ) {
                if   ( $U > $p0d01 ) { $X[$i] = 1 }
                else                 { $X[$i] = 0 }
            }
            else {
                if ( @X[ $i - 3 ] == 1 ) {
                    if   ( $U > $p0d001 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }
                else {
                    if   ( $U > $p0d000 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }

            }

        }

        if ( $X[$i] == 0 ) { $CM = $CM . $X[$i]; }
        $i++;
    }

    return ($CM);

}


sub simMod2 {
    my ($N)= @_;

    #1
    $p0d1 = 1;
    $p1d1 = 0;

    #10
    $p0d01 = 0.2;

    #000
    $p0d000 = 0.4;

    #100
    $p0d001 = 0.3;

    @X[1] = 1;
    @X[0] = 1;
    $CM   = $X[1];

    for ( $i = 2 ; $i <= ( $N + 1 ) ; $i++ ) {
        $U = rand(1);

        if ( @X[ $i - 1 ] == 1 ) { $X[$i] = 0 }
        else {
            if ( @X[ $i - 2 ] == 1 ) {
                if   ( $U > $p0d01 ) { $X[$i] = 1 }
                else                 { $X[$i] = 0 }
            }
            else {
                if ( @X[ $i - 3 ] == 1 ) {
                    if   ( $U > $p0d001 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }
                else {
                    if   ( $U > $p0d000 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }

            }

        }

        if ( $i > 0 ) { $CM = $CM . $X[$i]; }
    }

    while ( @X[ $i - 1 ] != 1 ) {
        $U = rand(1);

        if ( @X[ $i - 1 ] == 1 ) { $X[$i] = 0 }
        else {
            if ( @X[ $i - 2 ] == 1 ) {
                if   ( $U > $p0d01 ) { $X[$i] = 1 }
                else                 { $X[$i] = 0 }
            }
            else {
                if ( @X[ $i - 3 ] == 1 ) {
                    if   ( $U > $p0d001 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }
                else {
                    if   ( $U > $p0d000 ) { $X[$i] = 1 }
                    else                  { $X[$i] = 0 }
                }

            }

        }

        if ( $X[$i] == 0 ) { $CM = $CM . $X[$i]; }
        $i++;
    }

    return ($CM);

}



sub CTM {
    my ( $data, $Dmax, $LA, $l_sep, $c )= @_;
    %QNODE       = ();
    %PNODE       = ();
    %CONDNODE    = ();
    %PROB        = ();
    %FINALTREE   = ();
    %CONDCONTEXT = ();
    $LD          = length($data);

    $Pena = $c;
    for ( $l = 1 ; $l <= $Dmax ; $l++ ) {
        for ( $n = 0 ; $n < ( $LA**$l ) ; $n++ ) {

            @d[0] = $n % $LA;
            $ss = @d[0];
            for ( $k = 1 ; $k < $l ; $k++ ) {
                $r = ( $n - $ss ) / ( $LA**$k );
                @d[$k] = $r % $LA;
                $ss = $ss + @d[$k] * $LA**$k;
            }
            $node = "";
            for ( $k = 0 ; $k < $l ; $k++ ) {
                $node = $node . @l_sep[ @d[$k] ];
            }

            #	  print "$n $node\n";
            $ancester = int( $n / $LA );
            $smatch   = "?=" . $node;
            $NS       = $data =~ s/($smatch)/$1/g;

            $NS    = 1 * $NS;
            $LPMLS = 0;
            if ( $NS > 0 ) {
                for ( $j = 0 ; $j < $LA ; $j++ ) {
                    $smatch  = "?=" . $node . @l_sep[$j];
                    $count   = $data =~ s/($smatch)/$1/g;
                    @NSa[$j] = 1 * $count;

                    @PSa[$j] = @NSa[$j] / $NS;
                    if ( @NSa[$j] > 0 ) {
                        $LPMLS = $LPMLS + @NSa[$j] * log( @PSa[$j] );
                    }
                }
            }
            else {
                for ( $i = 0 ; $i < $LA ; $i++ ) { @PSa[$i] = 0; }
            }
            $LPS = $LPMLS - log($LD) * ( ( $LA - 1 ) * $Pena );
            $PS = $NS / ( $LD - $l + 1 );
            @LINE[$n] = ( [ $ancester, $node, $NS, $LPS, $cero, [@PSa], $PS ] );
            $terminal = 1 * ( $l == $Dmax );
        }
        @TREE[$l] = [@LINE];
    }

    for ( $l = ( $Dmax - 1 ) ; $l > 0 ; $l-- ) {
        for ( $n = 0 ; $n < ( $LA**$l ) ; $n++ ) {
            if ( $TREE[$l][ ($n) ][2] > 1 ) {
                $m     = ($LA) * $n;
                $SLVaS = 0;
                $nrch  = 0;
                for ( $j = 0 ; $j < $LA ; $j++ ) {
                    if ( $TREE[ $l + 1 ][ ( $m + $j ) ][2] > 1 ) {
                        $SLVaS = $SLVaS + ( $TREE[ $l + 1 ][ ( $m + $j ) ][3] );
                        $nrch++;
                    }
                }
                $lps = $TREE[$l][$n][3];
                if ( $nrch > 1 ) {
                    if ( $SLVaS > $lps ) {
                        $TREE[$l][$n][3] = $SLVaS;
                        $TREE[$l][$n][4] = 1;
                    }
                }

                $QNODE{ $TREE[$l][$n][1] }    = $TREE[$l][$n][4];
                $PNODE{ $TREE[$l][$n][1] }    = $TREE[$l][$n][6];
                $CONDNODE{ $TREE[$l][$n][1] } = ( $TREE[$l][$n][5] );
            }
        }
    }

  
    for ( $l = 1 ; $l <= $Dmax ; $l++ ) {
        for ( $n = 0 ; $n < ( $LA**$l ) ; $n++ ) {
            @d[0] = $n % $LA;
            $ss = @d[0];
            for ( $k = 1 ; $k < $l ; $k++ ) {
                $r = ( $n - $ss ) / ( $LA**$k );
                @d[$k] = $r % $LA;
                $ss = $ss + @d[$k] * $LA**$k;
            }
            $node = "";
            for ( $k = 0 ; $k < $l ; $k++ ) {
                $node = $node . @l_sep[ @d[$k] ];
            }
            if ( $l == 1 ) {
                if ( $QNODE{$node} == 0 ) {
                    $smatch             = "?=" . $node;
                    $NS                 = $data =~ s/($smatch)/$1/g;
                    $P                  = $NS / $LD;
                    $PROB{$node}        = $P;
                    $FINALTREE{$node}   = 1;
                    $CONDCONTEXT{$node} = $CONDNODE{$node};
                }
            }
            else {
                $nodep = 1;
                for ( $m = 1 ; $m < $l ; $m++ ) {
                    $lmm     = $l - $m;
                    $subnode = substr( $node, $m, $lmm );
                    $nodep   = $nodep * $QNODE{$subnode};
                }
                if ( $nodep > 0.1 ) {
                    if ( $QNODE{$node} == 0 ) {

                        $smatch = "?=" . $node;
                        $NS     = $data =~ s/($smatch)/$1/g;
                        $P      = $NS / $LD;
                        if ( $P > 0 ) {
                            $FINALTREE{$node}   = 1;
                            $PROB{$node}        = $P;
                            $CONDCONTEXT{$node} = $CONDNODE{$node};
                        }
                    }
                }
            }
        }
    }


    $tamarbolviejo = $tamarbol;

    # print "$Pena\n";
    $ncont = 0;

 
    $thistree = "";
    foreach $k1 ( sort keys %FINALTREE ) {
        $thistree = $thistree . " " . $k1;

        @context[$ncont]     = $k1;
        @probcontext[$ncont] = $PROB{$k1};
        @condcontext[$ncont] = $CONDCONTEXT{$k1};

        $ncont++;
    }
    $tamarbol = $ncont;

    return ($thistree);
}

sub LML {
    my ( $data, $LA, $l_sep, $tree )= @_;
  
    $LD          = length($data);
    @contextos    = split( / /, $tree );
   $Ncontextos=@contextos;
	$LPMLS=0;
	
    for ( $l = 1 ; $l < $Ncontextos ; $l++ ) {
		$node=$contextos[$l];
	   	$smatch   = "?=" . $node;
       	$NS       = $data =~ s/($smatch)/$1/g;
		$NS    = 1 * $NS;
		if($NS>=1){
		for ( $j = 0 ; $j < $LA ; $j++ ) {
                $smatch  = "?=" . $node . @l_sep[$j];
                $nsa  = $data =~ s/($smatch)/$1/g;
                @NSa[$j] = 1 * $nsa;
                @PSa[$j] = @NSa[$j] / $NS;
                if ( @NSa[$j] > 0 ) {
                    $LPMLS = $LPMLS + @NSa[$j] * log( @PSa[$j] );
                }
      	}
		}
	}
	
    return ($LPMLS);
}

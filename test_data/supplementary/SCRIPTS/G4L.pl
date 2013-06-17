#!/usr/local/bin/perl
#   G4L.pl
#
# SYNOPSIS: Context tree selection using the Smallest Maximizer Criterion (SMC).
#
# AUTHORS: Antonio Galves, Charlotte Galves, Jesus E. Garcia, Nancy L. Garcia, Florencia Leonardi.
#
# REQUIREMENTS 
#
#   - MAC OS X. Tested on MAC OS X version 10.6.8. Should work in OS X Leopard.
#   
#   - R must be installed on the computer. The perl script call R-project to perform part of the Bootstrap procedure. 
#
#   - a text file named abcde.txt containing the alphabet should be on the same directory. 
#
# USAGE
#
#     perl G4L.pl name datafile deep nr_re epsilon min_c max_c 4_if_renov_point prop_Res_1 prop_Res_2 test_level 
#
# ARGUMENTS
#    
#     name    : suffix to the file containing the results.
#
#     datafile: ASCII file containing the sample.
#
#     deep    : maximum possible deep for the fitted context tree.
#
#     nr_re : number of resamples in the bootstrap procedure. See Reference.
#
#     epsilon : tolerance used when estimating the set of champion trees. See Reference. 
#
#     min_c   : minimum value for the constant c. See Reference.
#
#     max_c   : maximum value for the constant c. See Reference.
#
#     4_if_renov_point: this argument has to be set to 0. Except in the linguistic application in which the letter 4 of the alphabet is a renovation point. See Reference.
#
#     prop_Res_1 : proportion of the size of the sample corresponding to the size of the smaller resample. Usually 0.3, see Reference.
#
#     prop_Res_2 : proportion of the size of the sample corresponding to the size of the larger resample. Usually 0.9, see Reference.
#
#     test_level  : alpha level to use on the t-test. Usually 0.01, see Reference.
#
#
# DETAILS:
#
#   The datafile should be an ASCII file with the sequence of letters without any separation. As an example see the file "folha_novo_no_fasta.txt".
#
#   The file "dataset.AUX.name.txt" will contain a log of the procedure including the set of Champion Trees and the chosen tree.
#
# REFERENCE: Context tree selection and linguistic rhythm retrieval from written texts. ArXiv: 0902.3619v3, 2011. 
#
# EXAMPLE: perl G4L.pl BP folha.txt  4 200 0.01 0.1 400 1 .3 .9 .01
#
#
#

$nome       = $ARGV[0];
$datafile   = $ARGV[1];
$Dmax       = $ARGV[2];
$Bstr       = $ARGV[3];
$eps        = $ARGV[4];
$c_ini      = $ARGV[5];
$c_fin      = $ARGV[6];
$renov_en_4 = $ARGV[7];  # put $renov_en_4=0 (except in the linguistic example).
$PBSS0 		= $ARGV[8];
$PBSS1 		= $ARGV[9];
$alpha 		= $ARGV[10];

$MOD = $datafile;

if($eps<=0  || $eps>=1 ){print"\n\n0 < epsilon <1 ";exit;}
if($c_ini>=$c_fin  || $c_ini <=0 ){print"\n\n0 < min_c < max_c ";exit;}
if($PBSS0>=$PBSS1 || $PBSS0<=0 || $PBSS1>1){print"\n\n0 < prop_Res_1 < prop_Res_2 < 1";exit;}
if($alpha<=0  || $alpha>=1 ){print"\n\n0 < test_level <1 ";exit;}

if ( open( READ, "<abcde.txt" ) ) {
    $linea = <READ>;
    chomp($linea);
    $LA       = length($linea);
    $alphabet = $linea;
    @l_sep    = split( //, $linea );
    close(READ);
    for ( $i = 0 ; $i < $LA ; $i++ ) { @NA[$i] = $i; }
}else{print"\n\n abcde.txt file not found\n\n";exit;}


$data = "";
if ( open( DATA, "<$datafile" ) ) {
    while ( $linea = <DATA> ) { chomp($linea); $data = $data . $linea; }
    $LD = length($data);
    $penal = log($LD) * ( $LA - 1 ) / 2;
}else{print"\n\n $datafile file not found\n\n";exit;}
$N    = $LD;
$BSS0 = int( $PBSS0 * $N );
$BSS1 = int( $PBSS1 * $N );

@BSSS[0] = $BSS0; 
@BSSS[1] = $BSS1; 
$LBSSS = 2; 

print(
"\nName: $nome \nDatafile: $datafile \nDeep: $Dmax \nNr. Resamples: $Bstr\nEpsilon: $eps\nMinimum c: $c_ini \nMaximum c: $c_fin \n4 is renovation point?(1=yes): $renov_en_4\nAlphabet: @l_sep\nResampling sizes: @BSSS\nAlpha: $alpha\n"
);
open( AUX, ">$MOD.AUX.$nome.txt" );
print( AUX
"\nName: $nome \nDatafile: $datafile \nDeep: $Dmax \nNr. Resamples: $Bstr\nEpsilon: $eps\nMinimum c: $c_ini \nMaximum c: $c_fin \n4 is renovation point?(1=yes): $renov_en_4\nAlphabet: @l_sep\nResampling sizes: @BSSS\nAlpha: $alpha\n"
);
close(AUX);

open( CyT, ">$MOD.CyTREES.$nome.$N.$Nsim.txt" );
close(CyT);
open( NUESTRO, ">$MOD.ArboreEscolhida.$nome.bs.lml.$N.$Nsim.txt" );
close(NUESTRO);
$Ncampeon = 0;
$NBIC     = 0;
$NUESTRO  = 0;

#Inicia

$BICtree = &CTM( $data, $Dmax, $LA, $l_sep, 0.5 );

print("\nTree chosen by the BIC procedure:\n$BICtree\n \nChampion trees:\n");

open( AUX, ">>$MOD.AUX.$nome.txt" );
print( AUX "\nTree chosen by the BIC procedure:\n$BICtree\n \nChampion trees:\n" );

#Looking for champion trees...
$Ntr   = 0;
$A     = $c_ini;
$B     = $c_fin;
$Atree = &CTM( $data, $Dmax, $LA, $l_sep, $A );
$Btree = &CTM( $data, $Dmax, $LA, $l_sep, $B );

@chtree[$Ntr] = $Atree;
print("$Ntr c=$A Tree[$A]=@chtree[$Ntr]\n ");
print( AUX "$Ntr c=$A Tree[$A]=$chtree[$Ntr]\n" );

print("\n");

open( NUESTRO,   ">>$MOD.ArboreEscolhida.$nome.bs.lml.$N.$Nsim.txt" );
print( NUESTRO "$ns \n" );
open( CyT, ">>$MOD.CyTREES.$nome.$N.$Nsim.txt" );
print( CyT "$ns. $Ntr c=$A Tree[$A]=@chtree[$Ntr]\n" );
print( NUESTRO "$Ntr:@chtree[$Ntr] ," );

$Ntr++;
while ( $Atree ne $Btree ) {
    $dif = $B - $A;
    while ( $dif > $eps ) {
        while ( $Atree ne $Btree ) {
            $Bold     = $B;
            $Boldtree = $Btree;
            $B        = ( $A + $B ) / 2;
            $Btree    = &CTM( $data, $Dmax, $LA, $l_sep, $B );

        }
        $A     = $B;
        $B     = $Bold;
        $Btree = $Boldtree;
        $dif   = $B - $A;
    }

    $A            = $B;
    $Atree        = $Btree;
    @chtree[$Ntr] = $Atree;

    print( AUX "$Ntr c=$B Tree[$B]=$chtree[$Ntr]\n" );
    print( CyT "$ns. $Ntr c=$B Tree[$B]=@chtree[$Ntr]\n" );
    print( NUESTRO "$Ntr:@chtree[$Ntr] ," );
	print("$Ntr c=$A Tree[$A]=@chtree[$Ntr]\n ");

    $Ntr++;
    $B = $c_fin;
    $Btree = &CTM( $data, $Dmax, $LA, $l_sep, $B );

}
print( CyT "\n" );
print( NUESTRO "\n" );
close(NUESTRO);
close(CyT);
############# end looking for champion trees#########

if ( $renov_en_4 != 1 ) {
############ Preparando el Bootstrap: separa bloques usando la sequencia de profundidade maxima con mayor frecuencia como punto de renvacion!!!
    $freq_nodehf = 0;
    for ( $n = 0 ; $n < ( $LA**$l ) ; $n++ ) {
        @d[0] = $n % $LA;
        $ss = @d[0];
        for ( $k = 1 ; $k < $l ; $k++ ) {
            $r = ( $n - $ss ) / ( $LA**$k );
            @d[$k] = $r % $LA;
            $ss = $ss + @d[$k] * $LA**$k;
        }
        $node = "";
        for ( $k = 0 ; $k < ( $l - 1 ) ; $k++ ) {
            $node = $node . @l_sep[ @d[$k] ];
        }
        $smatch = "?=" . $node;
        $NS     = $data =~ s/($smatch)/$1/g;
        $NS     = 1 * $NS;
        if ( $NS > $freq_nodehf ) { $freq_nodehf = $NS; $nodehf = $node; }
    }

    print(
"\nBootstrap:\nsplitting in blocks using the seq: $nodehf   (freq= $freq_nodehf)\n"
    );
    print( AUX
"\nBootstrap:\nsplitting in blocks using the seq: $nodehf   (freq= $freq_nodehf)\n"
    );

    @split_data = split( /$nodehf/, $data );
    $nut = @split_data;
    for ( $iseq = 1 ; $iseq <= $nut ; $iseq++ ) {
        @split_data[$iseq] = $nodehf . @split_data[$iseq];
    }
    print("\nNr. Blocks= $nut\n");
    print( AUX "\nNr. Blocks= $nut\n" );
}

if ( $renov_en_4 == 1 ) {
    @split_data = split( /4/, $data );
    $smatch     = "?=4";
    $nut        = @split_data;
    print("\nBootstrap:\nsplitting in blocks using 4 as a renovation point.\n");
    print( AUX "\nBootstrap: splitting in blocks using 4 as a renovation point.\n" );

    for ( $iseq = 1 ; $iseq <= $nut ; $iseq++ ) {
        @split_data[$iseq] = '4' . @split_data[$iseq];
    }
    print("\nNr. Blocks= $nut\n");
    print( AUX "\nNr. Blocks= $nut\n" );

}

close(AUX);

############ Bootstrap ###########################

for ( $nbsss = 0 ; $nbsss < $LBSSS ; $nbsss++ ) {
    print("Bootstrap size=$BSSS[$nbsss]\n");
    if ( $nbsss == 0 ) {
        open( BS0, ">$MOD.$nome.bs.lml.$BSSS[$nbsss].$N.$Nsim.txt" );
        print( BS0 "@cch\n" );
    }
    if ( $nbsss == 1 ) {
        open( BS1, ">$MOD.$nome.bs.lml.$BSSS[$nbsss].$N.$Nsim.txt" );
        print( BS1 "@cch\n" );
    }
}

for ( $bi = 1 ; $bi <= $Bstr ; $bi++ ) {
    $datacompl = &reamostra( $split_data, $nut, $BSSS[ ( $LBSSS - 1 ) ] );
    if ( $bi == 2 ) {
}
    for ( $nbsss = 0 ; $nbsss < $LBSSS ; $nbsss++ ) {
        $data = substr( $datacompl, 0, $BSSS[$nbsss] );
        @LMLT[0] = &LML( $data, $LA, $l_sep, $chtree[0] );

        #print"LMLT(0)=@LMLT[0]";
        for ( $nt = 1 ; $nt < $Ntr ; $nt++ ) {
            @LMLT[$nt] = &LML( $data, $LA, $l_sep, $chtree[$nt] );

            #print"nt=$nt, LMLT($nt)=@LMLT[$nt]";
        }
        if ( $nbsss == 0 ) { print( BS0 "@LMLT\n" ); }
        if ( $nbsss == 1 ) { print( BS1 "@LMLT\n" ); }

    }
}
if ( $nbsss == 0 ) { close(BS0); }
if ( $nbsss == 0 ) { close(BS1); }

$SEQTR = "";
for ( $nt = 0 ; $nt < $Ntr ; $nt++ ) {
    $ntm1  = ( $nt + 1 );
    $SEQTR = $SEQTR . "chtree[$ntm1] <- \"$chtree[$nt]\"
";
}

$comando =
"bs1<-read.csv(file=\"$MOD.$nome.bs.lml.$BSSS[0].$N.$Nsim.txt\",sep=\" \",header=FALSE)
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
print( RES "$comando\n" );
close(RES);
$linea = "R CMD BATCH $MOD.BS.r";

system $linea;

open( ULT, "<$MOD.Ultima.ArvoreEscolhida.txt" );
$linea = <ULT>;
chomp($linea);
close(ULT);
print("\nTree chosen by the SMC procedure:\n$linea \n");
open( ULT, "<$MOD.resultados.$nome.bs.lml.txt" );
@todo = <ULT>;
close(ULT);
open( AUX, ">>$MOD.AUX.$nome.txt" );
print( AUX "\n\nTree chosen by the SMC procedure:\n$linea \n" );
close(AUX);
$delete = " rm $MOD.Ultima.ArvoreEscolhida.txt
rm $MOD.ArboreEscolhida.$nome.bs.lml.txt
rm $MOD.$nome.bs.lml.$BSSS[0].$N.$Nsim.txt
rm $MOD.$nome.bs.lml.$BSSS[1].$N.$Nsim.txt
rm $MOD.resultados.$nome.bs.lml.$N.$Nsim.$ns.pdf
rm $MOD.resultados.$nome.bs.lml.txt
rm $MOD.ArboreEscolhida.$nome.bs.lml.$N.$Nsim.txt
rm $MOD.BS.r
rm $MOD.BS.r.Rout
rm $MOD.CyTREES.$nome.$N.$Nsim.txt

";
system $delete;

sub reamostra {
    my ( $split_data, $nut, $N ) = @_;
    $data = "";

    #print"\n @split_data\n";
    $tamseq = 0;
    while ( $tamseq <= $N ) {
        $r = int( rand($nut) );

        #		print"$r $data: ";
        #$r=int(rand($count));
        $data   = $data . @split_data[$r];
        $tamseq = length($data);
    }


    $data = substr( $data, 0, $N );

    #	    print "\n$data\n";
    return ($data);
}

sub CTM {
    my ( $data, $Dmax, $LA, $l_sep, $c ) = @_;
    %QNODE       = ();
    %PNODE       = ();
    %CONDNODE    = ();
    %PROB        = ();
    %FINALTREE   = ();
    %CONDCONTEXT = ();
    $LD          = length($data);

    #	print"n Dmax LA $n, $Dmax $LA\n";
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

    #          printf("\n c=$Pena\n");
    #		   printf( CONTEXT "c=$Pena\n" );
    $thistree = "";
    foreach $k1 ( sort keys %FINALTREE ) {
        $thistree = $thistree . " " . $k1;

        #                          print "contexto: $k1, prob:$PROB{$k1}\n";
        @context[$ncont]     = $k1;
        @probcontext[$ncont] = $PROB{$k1};
        @condcontext[$ncont] = $CONDCONTEXT{$k1};

 #                      printf( CONTEXT "$k1 $PROB{$k1} $CONDCONTEXT{$k1}[0]" );
        $ncont++;
    }
    $tamarbol = $ncont;

    return ($thistree);
}

sub LML {
    my ( $data, $LA, $l_sep, $tree ) = @_;

    $LD         = length($data);
    @contextos  = split( / /, $tree );
    $Ncontextos = @contextos;
    $LPMLS      = 0;

    #	print"\n $tree \n @contextos \n $Ncontextos \n";

    for ( $l = 1 ; $l < $Ncontextos ; $l++ ) {
        $node = $contextos[$l];

        #		print"\n l=$l, contexto= $node\n";
        $smatch = "?=" . $node;
        $NS     = $data =~ s/($smatch)/$1/g;
        $NS     = 1 * $NS;
        if ( $NS >= 1 ) {
            for ( $j = 0 ; $j < $LA ; $j++ ) {
                $smatch  = "?=" . $node . @l_sep[$j];
                $nsa     = $data =~ s/($smatch)/$1/g;
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

sub LMLpdada {
    my ( $data, $LA, $l_sep, $tree ) = @_;

    $LD         = length($data);
    @contextos  = split( / /, $tree );
    $Ncontextos = @contextos;
    $LPMLS      = 0;

    #	print"\n $tree \n @contextos \n $Ncontextos \n";

    for ( $l = 1 ; $l < $Ncontextos ; $l++ ) {
        $node = $contextos[$l];

        #		print"\n l=$l, contexto= $node\n";
        $smatch = "?=" . $node;
        $NS     = $data =~ s/($smatch)/$1/g;
        $NS     = 1 * $NS;
        if ( $NS >= 1 ) {
            for ( $j = 0 ; $j < $LA ; $j++ ) {
                $smatch  = "?=" . $node . @l_sep[$j];
                $nsa     = $data =~ s/($smatch)/$1/g;
                @NSa[$j] = 1 * $nsa;
                @PSa[$j] = $CONDNODE{$node}[$j];

                #print("($node $j) PSa=$PSa[$j] Nsa=$NSa[$j] \n ");
                if ( @NSa[$j] > 0 ) {
                    if ( @PSa[$j] > 0 ) {
                        $LPMLS = $LPMLS + @NSa[$j] * log( @PSa[$j] );
                    }
                }
            }
        }
    }

    return ($LPMLS);
}

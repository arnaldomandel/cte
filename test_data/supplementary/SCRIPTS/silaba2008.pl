#!/usr/bin/perl -w
#
# SILABA.PL 
# ⁄ltima alteracao 23/6/2008 (por F. Leonardi)
# Criado por Miguel Galves (miguel.galves@ic.unicamp.br)
# Programa que etiqueta palavras segundo 
# numero de silabas e posicao do acento 
#

use strict;

use encoding "latin1";

use Encode;


# Sub rotina que verifica a presenca de enclise na palavra
# retornando um valor inteiro que correspondera a quanto
# se devera acrescentar para se saber a posicao correta do acento
sub verify_enc {
    
    my ($token) = @_;
    pos $token = 0;
    if ($token =~m/(\w)+-(me|te|se|lhe|o|a|nos|vos|lho|lha|mo|ma|to|ta)s?$/i) 
    {
	return 1;
    }
    if ($token =~ m/(\w)+-(no-lo|no-la|vo-lo|vo-la|se-lhe)s?$/i)
    {
	return 2;
    }
    return 0;
    
} # sub &verify_enc


###########################################################################

# sub rotina que aplica regras de acentuacao para
# palavras que nao possuem acento grafico
sub apply_rules {
    
    my ($token) = @_;
    my $enc = &verify_enc($token);
    my $sub_token = $token;
    # Verifica se existe enclise na palavra analisada
    # caso exista, separa a substring anterior aa enclise para analise
    if ($enc>0)
    {
	$sub_token = substr($token,0,index($token,"-"));
    }
    # Aplica as regras para palavras nao acentuadas
    if ($sub_token =~ m/(a|e|o|as|os|es|em|ens|am)$/i)
    {
	if ($enc == 0) {return "PAR";}
	else {return "PRO";}
    }
    else 
    {
	if ($enc == 0) {return "OXI";}
	if ($enc == 1) {return "PAR";}
	else {return "PRO";}
    }
} # sub &apply_rules


# ########################################################################

# sub rotina que aplica regras de acentuacao 
# para palavras que possuem acento grafico.
# Para isso, extrai uma substring do token inicial que inicia
# na primeira letra apos o acento ou ditongo tonico (se existir)
# contando o numero de silabas da substring.
sub find_accent {
    
    my $sub_sil = 0;
    my ($token, $num_sil, $pos_acent, $token_acent) = @_;
    my $sub_token = "";
    my $enc = &verify_enc($token); ## linha adicionada por Florencia
    
    # Extrai a substring iniciada apos o acento grafico.
    # Verifica se  a letra que segue a vogal acentuada nao e i ou u 
    # ou se a vogal acentuada nao e til
    if (($token !~ m/$token_acent([iu])/ig) && ($token_acent ne "„" && 
						$token_acent ne "ı")) 
    {
	$sub_token = substr ($token, $pos_acent+1, 
			     length($token)-($pos_acent+1));
    }
    else 
    {  
        $sub_token = substr ($token, $pos_acent+2, 
			     length($token)-($pos_acent+2));
    }
    # Conta o numero de vogais, e elimina os ditongos
    if (defined $sub_token)
    {
	while ($sub_token =~ m/[aeiouy]/gi) 
	{
	    ++ $sub_sil;
	}
	if ($sub_token =~ m/([aeiouy])([aeiouy])/gi) 
	{
	    -- $sub_sil;
	}
    }
    # Adiciona o numero de silabas da enclise
    $sub_sil = $sub_sil + $enc;
    if ($sub_sil == 0) {return "OXI";}
    if ($sub_sil == 1) {return "PAR";}
    if ($sub_sil >= 2) {return "PRO";}
    
} # sub &find_accent


# ########################################################################


# sub rotina que faz o trabalho mais importante
# do script : determina o numero de silabas.
sub parse_token {
    
    my $num_sil = 0;
    my $pos_acent = -1;
    my $acento = 0;
    my ($token) = @_;
    my $tag = "";
    my $sub_tag = "";
    my $token_acent;
    my $acent;
    our @acents;
    our $pal_fon;
    
    # procura as vogais  
    while ($token =~ m/[aeiouy]/gi) 
    {
	++ $num_sil;
    }
    
    # verifica a existencia de uma vogal acentuada
    # guardando sua posicao caso exista e verificando casos
    # onde e necessario reduzir o numero de silabas.
    foreach $acent (@acents)
    {
	if (($pos_acent = index($token, $acent))>-1)
	{
	    $token_acent = $acent;
	    $acento = 1;
	    $num_sil++;
	    pos $token = 0;
	    # Verifica se a vogal acentuada e um til, e se esta e precedida
	    # por o ou e, reduzindo o numero de vogais caso verdade
	    if ($acent eq '„' || $acent eq 'ı')		
	    {
		if ($token =~ m/$acent[oe]/ig)  
		{
		    -- $num_sil;
		}		
	    } 
	    # verifica se a vogal acentuada nao 
	    # pertence a um ditongo aberto tonico
	    else
	    {
		if ($token =~ m/$acent([iu])/ig)  
		{
		    -- $num_sil; 
		    
		}
	    }
	    last;
	}
    }
    
    # ####################################################################
    
    pos $token = 0; # Zera o ponteiro de posicao de $token
    
    # Bloco que analisa grupos de duas vogais
    # seguidas identificando como ditongos ou hiatos
    while ($token =~ m/(\w?(([aeiouy])([aeiouy])))/gi) 
    {
	if ($1 eq "que" || $1 eq "qua" || $1 eq "qui" || $1 eq "quo"  || 
	    $1 eq "gue" || $1 eq "gua" || $1 eq "gui" || $1 eq "guo")
	{
	    -- $num_sil;
	}
	else
	{
	    if (($acento==1 && index($token,$1, $pos_acent)>-1) ||
		(($2 ne "ia" && $2 ne "uo" && $2 ne "ua" && $2 ne "ae" 
                  && $2 ne "oa" && $2 ne "ea" 
		  && $2 ne "ie" && $2 ne "io" && $2 ne "oe" && $2 ne "ia" 
                  && $2 ne "eo" && $3 ne $4) 
		 && ($acento==0||($acento==1 
				  && index($token,$2)<$pos_acent))))
	    {
		-- $num_sil;
	    }
	}	
    }
    # Verifica a existencia de hiatos seguidos de l,m,n,r z que nao 
    #  iniciam vogal
    if (($acento == 0) && ($token =~ m/[^qg][aeiou](i|u)(l|m|n|r|z)
       ([^aeiouh]|$)/i)) 
    {
	++ $num_sil;
    }
    # Verifica a existencia de hiatos seguidos de nh
    if (($acento == 0) && ($token =~ m/([aeiou])(i|u)(nh)/i)) 
    {
	++ $num_sil;
    }
    
    # ####################################################################

    print "$num_sil\n";
    # Bloco que cria a etiqueta final...
    
    # Tratamento de erro :
    # caso o numero de silabas ao final da analise seja
    # inferior a 1, indicar o erro e terminar analise.
    if ($num_sil<1)
    {
	return $token;
    }
    # Criacao da etiqueta primaria paa palavras com mais de uma silaba
    if ($num_sil == 2) {$tag = "DIS";}
    if ($num_sil == 3) {$tag = "TRI";}
    if ($num_sil >= 4) {$tag = "POL$num_sil";}
    # Criacao da etiqueta secundaria para palavras de mais de uma silaba.
    # Verifica se a palava possue ou nao acento e chama a rotina 
    # apropriada
    if ($num_sil >1) 
    {
	if ($acento == 0) 
	{
	    $sub_tag = &apply_rules($token);
	}
	else 
	{
	    $sub_tag = &find_accent($token, $num_sil, $pos_acent, 
				    $token_acent);
	}
	$tag = "$tag-$sub_tag";
	
	if (($num_sil == 2) && ($sub_tag eq "PAR") && ($pal_fon == 0))
	{ print BIN ("10"); $pal_fon =1;}
	else {if (($num_sil == 2) && ($sub_tag eq "PAR") && ($pal_fon == 1))
	      {print BIN ("30");}}
	
	if (($num_sil == 2) && ($sub_tag eq "OXI") && ($pal_fon == 0))
	{ print BIN ("01"); $pal_fon=1;}
	else{ if (($num_sil == 2) && ($sub_tag eq "OXI") && ($pal_fon == 1))
	      { print BIN ("21");}}
	
	if (($num_sil == 3) && ($sub_tag eq "PRO") && ($pal_fon==0))
	{ print BIN ("100"); $pal_fon=1;}
	else{ if (($num_sil == 3) && ($sub_tag eq "PRO") && ($pal_fon==1))
	      { print BIN ("300");}}
	
	if (($num_sil == 3) && ($sub_tag eq "PAR") && ($pal_fon==0))
	{ print BIN ("010"); $pal_fon=1;}
	else{ if (($num_sil == 3) && ($sub_tag eq "PAR") && ($pal_fon==1))
	      { print BIN ("210");}}
	
	if (($num_sil == 3) && ($sub_tag eq "OXI") && ($pal_fon == 0))
	{ print BIN ("001"); $pal_fon=1;}
	else{ if (($num_sil == 3) && ($sub_tag eq "OXI")&& ($pal_fon == 1))
	      { print BIN ("201");}}
	
	if (($num_sil == 4) && ($sub_tag eq "PAR") && ($pal_fon == 0))
	{ print BIN ("0010"); $pal_fon=1;}
	else{ if (($num_sil == 4) && ($sub_tag eq "PAR") && ($pal_fon == 1))
	      { print BIN ("2010");}}
	
	if (($num_sil == 4) && ($sub_tag eq "PRO") && ($pal_fon == 0))
	{ print BIN ("0100"); $pal_fon=1;}
	else{ if (($num_sil == 4) && ($sub_tag eq "PRO") && ($pal_fon == 1))
	      { print BIN ("2100");}}
	
	if (($num_sil == 4) && ($sub_tag eq "OXI") && ($pal_fon==0))
	{ print BIN ("0001"); $pal_fon=1;}
	else{ if (($num_sil == 4) && ($sub_tag eq "OXI") && ($pal_fon==1))
	      { print BIN ("2001");}}
	
	if (($num_sil == 5) && ($sub_tag eq "PRO")&& ($pal_fon==0))
	{ print BIN ("00100"); $pal_fon=1;}
	else { if (($num_sil == 5) && ($sub_tag eq "PRO")&& ($pal_fon==1))
	       { print BIN ("20100");}}
	
	if (($num_sil == 5) && ($sub_tag eq "PAR") &&($pal_fon == 0))
	{ print BIN ("00010"); $pal_fon=1;}
	else{ if (($num_sil == 5) && ($sub_tag eq "PAR") &&($pal_fon == 1))
	      { print BIN ("20010");}}
	
	if (($num_sil == 5) && ($sub_tag eq "OXI")&& ($pal_fon==0))
	{ print BIN ("00001"); $pal_fon=1;}
	else { if (($num_sil == 5) && ($sub_tag eq "OXI")&& ($pal_fon==1))
	       { print BIN ("20001");}}
	
	if (($num_sil == 6) && ($sub_tag eq "PRO")&& ($pal_fon==0)) 
	{ print BIN ("000100"); $pal_fon=1;} 
	else { if (($num_sil == 6) && ($sub_tag eq "PRO")&& ($pal_fon==1)) 
	       { print BIN ("200100");}} 
	
	if (($num_sil == 6) && ($sub_tag eq "PAR") &&($pal_fon == 0)) 
	{ print BIN ("000010"); $pal_fon=1;} 
	else{ if (($num_sil == 6) && ($sub_tag eq "PAR") &&($pal_fon == 1)) 
	      { print BIN ("200010");}} 
	
	if (($num_sil == 6) && ($sub_tag eq "OXI")&& ($pal_fon==0)) 
	{ print BIN ("000001"); $pal_fon=1;} 
	else { if (($num_sil == 6) && ($sub_tag eq "OXI")&& ($pal_fon==1)) 
	       { print BIN ("200001");}} 
	
	if (($num_sil == 7) && ($sub_tag eq "PRO")&& ($pal_fon==0)) 
	{ print BIN ("0000100"); $pal_fon=1;} 
	else { if (($num_sil == 7) && ($sub_tag eq "PRO")&& ($pal_fon==1)) 
	       { print BIN ("2000100");}} 
	
	if (($num_sil == 7) && ($sub_tag eq "PAR") &&($pal_fon == 0)) 
	{ print BIN ("0000010"); $pal_fon=1;} 
	else{ if (($num_sil == 7) && ($sub_tag eq "PAR") &&($pal_fon == 1)) 
	      { print BIN ("2000010");}} 
	
	if (($num_sil == 7) && ($sub_tag eq "OXI")&& ($pal_fon==0)) 
	{ print BIN ("0000001"); $pal_fon=1;} 
	else { if (($num_sil == 7) && ($sub_tag eq "OXI")&& ($pal_fon==1)) 
	       { print BIN ("2000001");}} 
	
	if (($num_sil == 8) && ($sub_tag eq "PRO")&& ($pal_fon==0)) 
	{ print BIN ("00000100"); $pal_fon=1;} 
	else { if (($num_sil == 8) && ($sub_tag eq "PRO")&& ($pal_fon==1)) 
	       { print BIN ("20000100");}} 
	
	if (($num_sil == 8) && ($sub_tag eq "PAR") &&($pal_fon == 0)) 
	{ print BIN ("00000010"); $pal_fon=1;} 
	else{ if (($num_sil == 8) && ($sub_tag eq "PAR") &&($pal_fon == 1)) 
	      { print BIN ("20000010");}} 
	
	if (($num_sil == 8) && ($sub_tag eq "OXI")&& ($pal_fon==0)) 
	{ print BIN ("00000001"); $pal_fon=1;} 
	else { if (($num_sil == 8) && ($sub_tag eq "OXI")&& ($pal_fon==1)) 
	       { print BIN ("20000001");}} 

	if (($num_sil == 9) && ($sub_tag eq "PRO")&& ($pal_fon==0)) 
	{ print BIN ("000000100"); $pal_fon=1;} 
	else { if (($num_sil == 9) && ($sub_tag eq "PRO")&& ($pal_fon==1)) 
	       { print BIN ("200000100");}} 
	
	if (($num_sil == 9) && ($sub_tag eq "PAR") &&($pal_fon == 0)) 
	{ print BIN ("000000010"); $pal_fon=1;} 
	else{ if (($num_sil == 9) && ($sub_tag eq "PAR") &&($pal_fon == 1)) 
	      { print BIN ("200000010");}} 
	
	if (($num_sil == 9) && ($sub_tag eq "OXI")&& ($pal_fon==0)) 
	{ print BIN ("000000001"); $pal_fon=1;} 
	else { if (($num_sil == 9) && ($sub_tag eq "OXI")&& ($pal_fon==1)) 
	       { print BIN ("200000001");}} 

    } 
    # Caso especial : monossilabos.
    else
    {  
	# Bloco executado caso o monossilabo nao possua acento grafico
	if ($acento == 0)
	{
	    if ($token =~ m/^(mo|to|me|te|se|na|nas|no|lhe|o|a|nos
				  |vos|lho|lha|um|uns|sem|com|me|mas|ou|
				  |por|de|‡|ao|do|da|que|se|e|em|ens|per
				  |no-lo|no-la|vo-lo|vo-la)s?$/i) 
	    {		 
		$tag = "MA";
		if ($pal_fon==1)
		{print BIN ("2"); $pal_fon=0;}
		else{ { print BIN ("0");}}
	    }
	    else 
	    {
		$tag = "MT";
		if ($pal_fon==1)
		{print BIN ("3"); }
		else{ { print BIN ("1"); $pal_fon = 1;}}
	    }
	    # Caso ao monossilabo possua acento grafico
	}
	else {$tag = "MT";
	      if ($pal_fon==1)
	      {print BIN ("3");}
	      else{ { print BIN ("1"); $pal_fon = 1;}}
	      
	}
	
    }
    #return "/$tag";
} # sub &parse_token


# ######################################################################
# Bloco principal do programa

# abre os arquivos de dados
print "Silabas 1.5 - Por Miguel Galves.\n";
print "(miguel.galves\@ic.unicamp.br)\n";
my $fin = $ARGV[0];
my $sub_fin;
my $fbin;
our @acents;

if (rindex($fin, '.')!= -1)
{
    $sub_fin = substr ($fin, 0, rindex($fin, '.'));
}
else {$sub_fin = $fin;}

$fbin = "$sub_fin.cod";

if (!open(IN, "<:encoding(latin1)", "$fin"))
{
    print "Arquivo $fin nao existente\n";
    exit;
}
print "Arquivo de entrada : $fin \n";

open(BIN, ">$fbin");

# inicializa variaveis globais
@acents = split (//, "·ÈÌÛ˙Ô¸‚ÍÓÙ˚„ı‡¡…Õ”⁄Œ‘¬ €√’¿œ‹");
# le todas as linhas do arquivo de entrada
# armazenando as em um vetor
# @lines = <IN>;
# separa cada linha em tokens delimitados por
#  .,;()-?!<>
print "Iniciando analise do arquivo $fin...\n";
my $contador = 0;
our $pal_fon = 1;
# imprime cabecalho fasta
print BIN ("\> $fin\n");
my $line;
my @tokens;
my $token;
while ($line = <IN>)
{
    $line =~ tr/<.+>?.+<?>//;
    @tokens = split (/([\s\.\?!,;:()])/, $line);
    # quebra uma linha em varios tokens
    foreach $token (@tokens)
    {   
	if ($token =~ m/[^\s\.\!\?(),;:]/i)
	{
	    $contador++;
	    &parse_token (lc $token); # Analise do token    
	}
	else 
	{			
	    if ($token eq "." || $token eq "!" || $token eq "?")
	    {
                print BIN ("4\n" );
		$pal_fon = 1;
	    }
	}
    }
}

close (IN);
close (BIN);

print "Analise terminada. $contador palavras analisadas.\n";


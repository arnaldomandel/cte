# Variables set by plotdl
# Time-stamp: <2013/05/04 22:12:15 hutzpa [hutzpa] am>

#plot for [i=From:From+(Npp-1)*Inc:Inc] file index i using col title sprintf("%d",i) smooth kdensity
plot for [i=From:From+(Npp-1)*Inc:Inc] file index i using col title sprintf("%d",i) with linesp
if (nofile) pause(Pause)
From=From+Delta
if(From < To) reread

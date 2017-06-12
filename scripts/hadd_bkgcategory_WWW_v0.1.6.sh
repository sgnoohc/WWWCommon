if [ -z $1 ]; then
  echo "Must provide the directory path to perfomr the hadding"
  exit
fi

ditto_hadd \
  '-!' '2,www'                      -1 'www_2l'     \
  '-@' '4,vh'                       -2 'vh_nonbb'   \
  '-A' '2001,true'                  -a '_true__'    \
  '-B' '2007,charge-flip'           -b '_chflip__'  \
  '-C' '2003,lost-lep'              -c '_lostlep__' \
  '-D' '2005,one-fake'              -d '_onefake__' \
  '-E' '2006,two-fake'              -e '_twofake__' \
  -O ${1}/haddoutput -o ${1}/output/

#	  -2 'www_2l' '-@' wwwmia \
#	  -3 'vh_nonbb,www_2l' '-#' wwwboth \

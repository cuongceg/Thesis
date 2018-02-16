for f in *txt
do
	../a.out -p 1 -k 1 < $f > "${f%.txt}.svg"
done



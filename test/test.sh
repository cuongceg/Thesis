for f in *txt
do
	../a.out < $f > "${f%.txt}.svg"
done



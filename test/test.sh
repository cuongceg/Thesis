for f in *txt
do
	../a.out < $f | python ../drawGraph.py > "${f%.txt}.svg"
done



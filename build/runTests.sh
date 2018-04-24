cd tests/input;
for file in *
do
	../.././zipio "$file" >> ../../testRun.out
done
cd ..;
mv input/*.zipio output/;
cd output/;
for file in *
do
	../.././zipio "$file" >> ../../testRun.out
done
cd ..;
mv output/*.txt output2/;
diff input output2 > ../differences.txt
cd ..;

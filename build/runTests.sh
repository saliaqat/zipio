cd tests/input;
for file in *
do
	../.././zipio "$file" >> ../../testRun.out
done
cd ..;
mv input/*.zipio output/;
cd ..;

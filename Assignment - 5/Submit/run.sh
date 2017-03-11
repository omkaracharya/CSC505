g++ sweep.cpp -o sweep -std=c++98
./sweep < input_s1.txt > output_s1.txt
./sweep < input_s2.txt > output_s2.txt
./sweep < input_s3.txt > output_s3.txt
./sweep < input_ec1.txt > output_ec1.txt
diff output_s1.txt expected_s1.txt
diff output_s2.txt expected_s2.txt
diff output_s3.txt expected_s3.txt
diff output_ec1.txt expected_ec1.txt
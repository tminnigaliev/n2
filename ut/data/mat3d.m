a = zeros(2,3,4);
n = 0;

for k=(1:4)
	for j=(1:3)
		for i=(1:2)
			a(i,j,k) = n;
			n = n+1;
		end
	end
end

keyboard

save "a.txt" a

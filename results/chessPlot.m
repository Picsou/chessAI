data = load ("history_individuals.txt");
iterations = 2292;
v = [1:iterations];
hold on;
xlabel ("O(iterations)");
ylabel ("Values");
title ("Evolution of parameters through iterations");
plot (v, data (:,2), "r", v, data (:,3), "g", v, data (:,4), "b",v, data (:,5), "m", v, data (:,6), "c");
legend ({"Pawn", "Knight", "Bishop", "Rook", "Queen"});
print("history_individuals.png")

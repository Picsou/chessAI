data = load ("history_individuals_2.txt");
iterations = 216;
v = [1:iterations];
hold on;
xlabel ("O(iterations)");
ylabel ("Values");
title ("Evolution of parameters through iterations");
%plot (v, data (:,2), "r");
%plot (v, data (:,3), "g");
%plot (v, data (:,4), "b");
%plot (v, data (:,5), "m");
%plot (v, data (:,6), "c");
plot (v, data (:,2), "r", v, data (:,3), "g", v, data (:,4), "b",v, data (:,5), "m", v, data (:,6), "c");
legend ({"Pawn", "Knight", "Bishop", "Rook", "Queen"});
%legend('show');
print("history_individuals_2.png")

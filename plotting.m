% 	int trials;			/* number of trials to run					*/
% 	int popsize;		/* number of individuals in each generation	*/
% 	int maxGen;			/* number of generations					*/
% 	int nCrossover;		/* number of crossover points				*/
% 	float pCrossover_s;	/* starting probability of crossover		*/
% 	float pCrossover_e;	/* ending probability of crossover			*/
% 	float pMutation_s;	/* starting probability of mutation			*/
% 	float pMutation_e;	/* ending probability of mutation			*/
% 	int elitesize_s;	/* starting number of elites				*/
% 	int elitesize_e;	/* ending number of elites					*/
% 	int ord;			/* ordinary of the test set					*/

trials = 30;
popsize = 20;
maxGen = 50;
nCrossover = 1;
pCrossover_s = 1;
pCrossover_e = 0.8;
pMutation_s = 0.02;
pMutation_e = 0.0001;
elitesize_s = 1;
elitesize_e = 15;
ordinary = 3;

cmd = sprintf('./ga.x -t %d -p %d -g %d -n %d -C %d -c %d -M %d -m %d -E %d -e %d -o %d', ...,
    trials, popsize, maxGen, nCrossover, pCrossover_s, pCrossover_e, ...,
    pMutation_s, pMutation_e, elitesize_s, elitesize_e, ordinary)
%eval(cmd);
fittests = load('../result/fittest_03.txt');
plot((1:500) * 20, fittests, 'b-')
legend('GA');
xlabel('function evaluations');
ylabel('average cost over all trials');
title('Genetic Algorithm performence');
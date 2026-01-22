AlgsNames = ['BSA       ','MSSA_BSA      ','ISA     ','MSSA_ISA    '];

disp(['AlgsNames = ',AlgsNames]);
disp(['ExecTime = ',num2str(ExecutionTime)]);

c = categorical({'BSA','MSSA\_BSA','ISA','MSSA\_ISA'});
figure
bar(c,ExecutionTime);
title(['ExecTime for searching ', num2str(m), 'items (all existed)']);
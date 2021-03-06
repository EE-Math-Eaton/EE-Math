library(data.table)
library(dplyr)

plotFile <- read.csv(file.choose(),header=FALSE,sep=",")
data <- plotFile[,5:14]
cols <- c("+4","+3","+2","+1","+0","-1","-2","-3","-4","+1.5")
colnames(data) <- cols
data_table <- as.data.table(data, TRUE)
data_table <- data_table %>% select(-rn)
data_table <- t(data_table)

barplot(data_table,
        main = "Monte Carlo Distribution",
        xlab = "Starting Condition",
        ylab = "Distribution",
	col = c("#000066", "#2222aa",
		"#4444dd", "#6666ff",
		"#aa88aa", "#ff6666",
		"#dd4444", "#aa2222",
	"#660000", "#88dd88"),
        border = NA,
	space = 0,
	legend.text = TRUE, 
        args.legend = list(x="bottom", inset=c(0,-0.125), bty="n", ncol=5),
        axes = TRUE)

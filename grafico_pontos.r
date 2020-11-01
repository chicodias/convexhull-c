library(tidyverse)
library(ggpubr)
library(gridExtra)

a <- seq(1:100000) %>% tibble()
a <- tibble(x= a, sq= a^2, lg = a*log(a))

a %>% ggplot() + geom_line(aes(x=x, y = sq))


fun.1 <- function(x) x^2

fun.2 <- function(x) x*log(x)


n = c(100000,
  200000,
  300000,
  400000,
  500000,
  600000,
  700000,
  800000,
  900000,
  1000000)

wrap <- c(0.53,
  1.20,
  2.92,
  3.60,
  4.41,
  5.89,
  7.29,
  8.85,
  9.39,
  10.31)


graham <- c(0.10,
  0.29,
  0.63,
  0.93,
  1.26,
  1.85,
  2.04,
  2.22,
  2.66,
  2.96)


tempo <- tibble(n, wrap, graham)

p<- tempo %>% ggplot() + geom_point(aes(x = n, y = wrap, color = "blue")) +
  geom_point(aes(x = n, y = graham, color = "green"))+
  theme_pubclean() +
  xlab("Tamanho da entrada") + ylab("Tempo (s)") + # Set axis labels
  ggtitle("Tempo Empírico ") +
  scale_color_discrete(name = "Algoritmo", labels = c("Embrulho", "Graham"))
p  
q<- ggplot(data = data.frame(x = 0), mapping = aes(x = x))+
  stat_function(linetype= "dashed",aes(color = "blue",),fun = fun.1) + stat_function(linetype= "dashed",aes(color = "red"), fun = fun.2) + xlim(0,10) +
  theme_pubr()+xlab("n") + ylab("Tempo") +
  ggtitle("Análise Assintótica") +
  rremove("x.text") + rremove("y.text") + rremove("ylab") +
  scale_color_discrete(name = "Complexidade", labels = c("O (n ²)","O(n log(n))"))
q
gridExtra::grid.arrange(p,q,ncol =2)

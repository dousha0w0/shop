# shop
你是⼀家药店的老板，这个⽉你从供货商⼿⾥收到了⼀批共50个药品， 其中每个药品有独立的进价和保质期，其中进价的区间为[20,30]元，保质期的剩余天数为[1,15]天 你每天可以陈列出10个药品在商店中售卖，每天会有三个顾客各过来购买⼀个药品。 药品的定价只能为商品进价加上{-1.5,-1, -0.5, 0, 2 ,4 ,6}元，不得随意定价。 每位顾客购买的逻辑是，买其中最便宜的药品，如果说最便宜的药品价格⼀致则购买保质期⻓的药品。 三位顾客会依次购买药品。 药品如果没有陈列在商店中，⽽是存放在仓库时，会收取管理费，其中保质期低于5天的每天收取1元管理费，其余的每天收取0.5元。
每天的陈列药品可以少于10个
你的⽬标是，10天之后，你的利润（售出商品售价总和-售出商品进价总和-⽀付仓库的管理费⽤-10天内过期/丢弃商品的进价）最⼤。
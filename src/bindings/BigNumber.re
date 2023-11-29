type t;

[@mel.new] [@mel.module "bignumber.js"] 
    external big_number: string => t = "BigNumber" ;

[@mel.send] external to_number: t => int = "toNumber";
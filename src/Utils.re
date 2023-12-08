let uuusd_address = "KT1XRPEPXbZK25r3Htzp2o1x7xdMMmfocKNW";

type selected_token =
| XTZ
| UUSD;

let show_selected_token = (tk: selected_token): string => 
    switch tk {
        | XTZ => "XTZ"
        | UUSD => "uUSD"
    };
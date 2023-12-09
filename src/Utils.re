let uuusd_address = "KT1XRPEPXbZK25r3Htzp2o1x7xdMMmfocKNW";

let show_selected_token = (tk: Context.selected_token): string => 
    switch tk {
        | XTZ => "XTZ"
        | UUSD => "uUSD"
    };
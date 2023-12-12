type selected_token =
| NOTOKEN
| XTZ
| UUSD;

module Utils = {
    type t;

    type right_cell_status =
    | NoWallet
    | Typing
    | Send
    | Sending
    | Error

    // let uusd_address = "KT1XRPEPXbZK25r3Htzp2o1x7xdMMmfocKNW";
    let uusd_address = "KT1LkNWZgVYh3zdaRkBb9aNgLEFCjVJwEKu2";

    let show_selected_token = (tk: selected_token): string => 
        switch tk {
            | NOTOKEN => ""
            | XTZ => "XTZ"
            | UUSD => "uUSD"
        };
    
    let token_from_display = (value: string, token: selected_token): result(float, string) => {
        let exponent = switch token {
            | XTZ => 1_000_000.
            | UUSD => 10. ** 12.
            | _ => 0.
        };
        if (exponent == 0.) {
            Error("wrong token")
        } else {
            switch (value |> Belt.Float.fromString) {
                | None => Error("error")
                | Some(float_val) => Ok(float_val *. exponent)
            }
        }
    };

    let token_to_display = (value: int, token: selected_token): result(string, string) => {
        let exponent = switch token {
            | XTZ => 1_000_000.
            | UUSD => 10. ** 12.
            | _ => 0.
        };
        if (exponent == 0.) {
            Error("wrong token")
        } else {
            Ok(((value |> Belt.Float.fromInt) /. exponent) |> Belt.Float.toString)
        }
    };
}

type projectContext = 
{
    tezos: option(Taquito.TezosToolkit.t),
    set_tezos: (option(Taquito.TezosToolkit.t) => option(Taquito.TezosToolkit.t)) => unit,
    user_address: option(string),
    set_user_address: (option(string) => option(string)) => unit,
    user_xtz_balance: option(int),
    set_xtz_balance: (option(int) => option(int)) => unit,
    user_uusd_balance: option(int),
    set_uusd_balance: (option(int) => option(int)) => unit,
    selected_token: selected_token,
    set_selected_token: (selected_token => selected_token) => unit,
    amount_to_send: option(float),
    set_amount_to_send: (option(float) => option(float)) => unit,
    island_right_cell_status: Utils.right_cell_status,
    set_island_right_cell_status: (Utils.right_cell_status => Utils.right_cell_status) => unit
};

let context = React.createContext({ 
    tezos: None,
    set_tezos: _ => (),
    user_address: None,
    set_user_address: _ => (),
    user_xtz_balance: None,
    set_xtz_balance: _ => (),
    user_uusd_balance: None,
    set_uusd_balance: _ => (),
    selected_token: XTZ,
    set_selected_token: _ => (),
    amount_to_send: None, 
    set_amount_to_send: _ => (),
    island_right_cell_status: Utils.Send,
    set_island_right_cell_status: _ => ()
});

module Provider = {
    let makeProps = (~value, ~children, ()) => {"value": value, "children": children};
    let make = React.Context.provider(context);
};

// module Consumer = {
//   [@react.component]
//   let make = () => {
//     let (name, ) = React.useContext(ExampleContext.context).ExampleContext.name;
//     <div> {React.string("Hello " ++ name)} </div>;
//   };
// };

[@react.component]
let make = (~children) => {
    let (tezos, set_tezos) = React.useState(() => None)
    let (amount_to_send, set_amount_to_send) = React.useState(() => None);
    let (user_address, set_user_address) = React.useState(() => None);
    let (user_xtz_balance, set_xtz_balance) = React.useState(() => None);
    let (user_uusd_balance, set_uusd_balance) = React.useState(() => None);
    let (selected_token, set_selected_token) = React.useState(() => NOTOKEN);
    let (island_right_cell_status, set_island_right_cell_status) = React.useState(() => Utils.Send);

    <Provider value={ 
        tezos: tezos,
        set_tezos: set_tezos,
        user_address: user_address,
        set_user_address: set_user_address,
        user_xtz_balance: user_xtz_balance,
        set_xtz_balance: set_xtz_balance,
        user_uusd_balance: user_uusd_balance,
        set_uusd_balance: set_uusd_balance,
        selected_token: selected_token,
        set_selected_token: set_selected_token,
        amount_to_send: amount_to_send, 
        set_amount_to_send: set_amount_to_send,
        island_right_cell_status: island_right_cell_status,
        set_island_right_cell_status: set_island_right_cell_status
    }>
        children
    </Provider>;
};
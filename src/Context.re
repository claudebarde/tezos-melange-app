type selected_token =
| NOTOKEN
| XTZ
| UUSD;

let show_selected_token = (tk: selected_token): string => 
    switch tk {
        | NOTOKEN => ""
        | XTZ => "XTZ"
        | UUSD => "uUSD"
    };

type projectContext = 
{
    show_selected_token: selected_token => string,
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
    show_selected_token,
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
    let (amount_to_send, set_amount_to_send) = React.useState(() => None);
    let (user_address, set_user_address) = React.useState(() => None);
    let (user_xtz_balance, set_xtz_balance) = React.useState(() => None);
    let (user_uusd_balance, set_uusd_balance) = React.useState(() => None);
    let (selected_token, set_selected_token) = React.useState(() => NOTOKEN);
    let (island_right_cell_status, set_island_right_cell_status) = React.useState(() => Utils.Send);

    <Provider value={ 
        show_selected_token,
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
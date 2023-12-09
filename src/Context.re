type selected_token =
| XTZ
| UUSD;

type projectContext = 
{
    user_address: option(string),
    set_user_address: (option(string) => option(string)) => unit,
    user_xtz_balance: option(int),
    set_user_xtz_balance: (option(int) => option(int)) => unit,
    user_uusd_balance: option(int),
    set_user_uusd_balance: (option(int) => option(int)) => unit,
    selected_token: selected_token,
    set_selected_token: (selected_token => selected_token) => unit,
    amount_to_send: option(int),
    set_amount_to_send: (option(int) => option(int)) => unit
};

let context = React.createContext({ 
    user_address: None,
    set_user_address: _ => (),
    user_xtz_balance: None,
    set_user_xtz_balance: _ => (),
    user_uusd_balance: None,
    set_user_uusd_balance: _ => (),
    selected_token: XTZ,
    set_selected_token: _ => (),
    amount_to_send: None, 
    set_amount_to_send: _ => ()
    
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
    let (user_xtz_balance, set_user_xtz_balance) = React.useState(() => None);
    let (user_uusd_balance, set_user_uusd_balance) = React.useState(() => None);
    let (selected_token, set_selected_token) = React.useState(() => XTZ);

    <Provider value={ 
        user_address: user_address,
        set_user_address: set_user_address,
        user_xtz_balance: user_xtz_balance,
        set_user_xtz_balance: set_user_xtz_balance,
        user_uusd_balance: user_uusd_balance,
        set_user_uusd_balance: set_user_uusd_balance,
        selected_token: selected_token,
        set_selected_token: set_selected_token,
        amount_to_send: amount_to_send, 
        set_amount_to_send: set_amount_to_send
    }>
        children
    </Provider>;
};
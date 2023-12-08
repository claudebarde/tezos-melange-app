type styles_css = {. 
  "body": string,
  "cross": string,
  "check": string,
  "left": string,
  "center": string,
  "right": string,
  "small": string,
  "wide": string
};

type image = {. 
  "default": string
};

[@mel.module] external styles: styles_css = "./styles/Island.module.css";

[@mel.module "./assets/wallet-90.png"] external wallet_logo: string = "default";
[@mel.module "./assets/cross-90.png"] external cross_logo: string = "default";
[@mel.module "./assets/check-90.png"] external check_logo: string = "default";

[@react.component]
let make = (~center_text: option(string), ~connected: bool) => {
    let _ = Js.log(wallet_logo);
    <div className={styles##body ++ " " ++ (connected ? styles##wide : styles##small)}>
        <div className=styles##left>
            {
                (if (connected == true) {
                    <>
                        <img src=wallet_logo alt="wallet" />
                        <img src=check_logo alt="wallet" className=styles##check />
                    </>
                } else {
                    <>
                        <img src=wallet_logo alt="wallet" />
                        <img src=cross_logo alt="wallet" className=styles##cross />
                    </>
                })
            }
        </div>
        <div className=styles##center>{(
            switch center_text {
                | None => "No wallet"
                | Some(text) => text
            }
        )->React.string}</div>
        <div className=styles##right>{""->React.string}</div>
    </div>
}
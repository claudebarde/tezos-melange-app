[%%raw "import './index.css'"];

ReactDOM.querySelector("#root")
->(
    fun
    | Some(root) => ReactDOM.render(
        <Context>
          <App />
        </Context>, root)
    | None =>
      Js.Console.error(
        "Failed to start React: couldn't find the #root element",
      )
  );

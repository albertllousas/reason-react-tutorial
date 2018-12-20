# reason react - tutorial

The idea of this project is not to use a prebuild-template, instead what we want is to build a reason-react project from scratch, so we will be aware of all the steps that should be done in terms of building, coding, design patterns and running.

:construction: **Note:** Reason react, tool chain and testing tools are in an early stage an most of them are still experimental, so we should expect main changes and possibly this tutorial will become updated.

# Table of Contents
1. [Setup project](#setup-building)
    1. [Init project and tools](#init-reason-project-and-tool-chain)
    2. [Setup reason-react](#setup-reason-react)
    3. [Adding react code](#adding-react-code)
    4. [Web bundling](#web-bundling)
2. [Coding time](#coding-time)
    1. [Add testing dependencies](#add-testing-dependencies)
    2. [First iteration : show a remote random joke](#show-a-remote-random-joke)
        1. [Writing our first test](#writing-our-first-test)
        2. [Showing loader](#showing-loader)

# Setup project

## Init project and tools

```bash
mkdir fun-with-reason-react
cd fun-with-reason-react
npm init
```

To install BuckleScript (compiler that turns Reason code into JavaScript code) & Reason locally:

```bash
npm install --save-dev bs-platform
```
Generate a simple project template

```bash
./node_modules/bs-platform/lib/bsb -init . -theme basic-reason
```

This command should have generated a simple project scaffold with a simple file `src/Demo.re`:

```ocaml
Js.log("Hello, BuckleScript and Reason!");
```

Since we are have generated already a project we should update scripts in `package.json` in order to compile and run reason code:

```json
"scripts": {
    "test": "echo \"Error: no test specified\" && exit 1",
    "build": "bsb -make-world",
    "start": "bsb -make-world -w",
    "clean": "bsb -clean-world"
  },
```

Since we now have a .js file, we can run it with node.js:
```bash
npm run build
node src/Demo.bs.js
Hello, BuckleScript and Reason!
```

## Setup reason-react

Now we have to add react, in theory if we create a project from scratch with `bsb` and `-theme react` it would generate all the templating, but since we want to understand what we are doing let's do it manually.

First, install react:
```bash
npm install react react-dom reason-react
```

We should also change buckle script `bsconfig.json` to enable react builds adding a couple of lines, [more info](https://bucklescript.github.io/docs/en/build-configuration):

```json
"reason": {"react-jsx": 2},
"bs-dependencies": ["reason-react"],
```

## Adding react code

Now we should be able to add and compile some reason react code.

We will create a simple stateless react component in reason `HelloWorldReact.re`:

```ocaml
let component = ReasonReact.statelessComponent("HelloWorldReact");

let make = (_) => {
  ...component, /* spread the template's other defaults into here  */
  render: _self => <div> HelloWorld </div>
};

```
We should render it into the dom `Index.re`:
```ocaml
ReactDOMRe.renderToElementWithId(<HelloWorldReact />, "index");
```

And create a simple html to visualize it `index.html`:
```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Reason React - Hello world</title>
</head>
<body>
  <div id="index"></div>
  <script src="./Index.bs.js"></script>
</body>
</html>
```
Now compile and run!

```bash
npm run build
open src/index.html
```
And ... errors in the browser `Uncaught ReferenceError: require is not defined at Index.bs.js:4`

Sadly, we won't see anything because now we are just exposing a bs file but what we really need to make it work in the browser is add all the dependencies. We can do this in two different ways:

- Adding all the required scripts to the html (old way)
- Bundling all the dependecencies in one single js file (modern way)

## Web bundling

Bundling is the process to combines multiple files into a single file, besides that, optimization processes will be applied as well.

We will need a web bundler to automate all this process, so we will choose [Parcel](https://parceljs.org/), because why not?

```bash
npm install parcel-bundler --save-dev
```
Add bundle script to our `package.json`:

```json
{
  ...
  "scripts": {
   ...
   "serve": "parcel ./src/index.html",
   "bundle": "parcel build ./src/index.html"
  }
}
```

We can now bundle and serve our application in dev mode:

```bash
npm run serve
```

Or prepare the bundle for production:

```bash
npm run bundle
```

# Coding time

Let's create an small real app that consumes a public api:

- We will use https://icanhazdadjoke.com/api
- MVP, first iteration: We show a random joke.
- Second iteration: We will refresh the random joke with a button.

## Add testing dependencies

As a testing tools we want to use `jest` `react-test-renderer` and `enzyme`, but we should use the buckle-script bindings:

```bash
npm install --save-dev npm @glennsl/bs-jest bs-react-test-renderer bs-enzyme enzyme-adapter-react-16
```

Then add all the dependencies in your `bsconfig.json` as well:

```json
{
  ...
"bs-dependencies": ["reason-react", "@glennsl/bs-jest", "bs-enzyme", "bs-react-test-renderer"],
}
```

Add an script runner in `package.json`:

```json
{
  ...
  "scripts": {
   ...
   "test": "npm run build && jest",
   ...
  }
}
```

Create a simple test `__tests__/expect_test.re`, just to see if it worked:

```ocaml
open Jest

let () =

describe "Expect" (fun () -> 
  let open Expect in

  test "toBe" (fun () ->
    expect (1 + 2) |> toBe 3);
  )
);
```

## First iteration :  show a remote random joke

### Writing our first test
Now, we can do some TDD and write our first test `__tests__/RandomJoke_test.re`:

```ocaml

open Jest;

describe("<RandomJoke />", () => {
  open ExpectJs;

  test("render", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomJoke />);
    expect(Js.Undefined.return(component)) |> toBeDefined;
  });
});
```

We should create the component `src/RandomJoke.re`:

```ocaml
let component = ReasonReact.statelessComponent("RandomJoke");

let make = (_) => {
  ...component,
  render: _self => <div> {ReasonReact.string("todo")} </div>
};
```

Some notes:
- In OCaml/Reason all functions should be wrap into a modules `module MyModule = {...}`, but by default files are map to a module, so we don't need code it.
- In ReasonReact, instead of passing the whole "class" ReasonReact.createElement function, you'd instead declare a make function, it will be desugared to something like `ReasonReact.element(RandomJoke.make(...))`

And run the test:

```bash
npm test
 PASS  __tests__/RandomJoke_test.bs.js
  <RandomJoke />
    ✓ render (5ms)

Test Suites: 1 passed, 1 total
Tests:       1 passed, 1 total
Snapshots:   0 total
Time:        0.97s, estimated 1s
Ran all test suites.
```
Great! Now we are ready to add some production code!

### Showing loader

Since we want to show a remote joke, our second step should be to see a loading screen while we are fetching the data. To make it more interesting we will pass our loading message to the component `__tests__/RandomJoke_test.re`:

```ocaml
...
 test("snapshot while loading", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomJoke />);
    expect(Js.Undefined.return(component)) |> toMatchSnapshot;
  });

  test("snapshot while loading changing the default message", () => {
    let component = ReactShallowRenderer.renderWithRenderer(<RandomJoke loadingMessage="Wait for the joke ..."/>);
    expect(Js.Undefined.return(component)) |> toMatchSnapshot;
  });
```

Note: Since ReasonML doesn't have the notion of null or undefined, we should wrap our component with `Js.Undefined.return`. [More info](https://reasonml.github.io/docs/en/null-undefined-option)

And change the component: 
```ocaml
type state = {
  loading: bool
};

type action = Fetching;

let component = ReasonReact.reducerComponent("RandomJoke");

let make = (~loadingMessage="loading ...", _children) => {
  ...component,
  initialState: () => {loading: true},
  reducer: (action, state) => switch (action) {
    | Fetching => ReasonReact.Update({...state, loading: true})
    },
  render: self =>
    <div>(self.state.loading ? ReasonReact.string(loadingMessage): ReasonReact.null)</div>
};
```
Reason comes with "reducer" (like Redux) built in. So for small apps we can use it, for big apps there are some libraries like [reductive](https://github.com/reasonml-community/reductive).











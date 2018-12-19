# reason react - tutorial

The idea of this project is not to use a prebuild-template, instead what we want is to build a reason-react project from scratch, so we will be aware of all the steps that should be done in terms of building, coding, design patterns and running.

# Table of Contents
1. [Setup project](#setup-building)
    1. [Init project and tools](#init-reason-project-and-tool-chain)
    2. [Setup reason-react](#setup-reason-react)
    3. [Adding react code](#adding-react-code)
    4. [Web bundling](#web-bundling)
2. [Coding time](#coding-time)

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

```reasonml
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

```reasonml
let component = ReasonReact.statelessComponent("HelloWorldReact");

let make = () => {
  ...component, /* spread the template's other defaults into here  */
  render: _self => <div> HelloWorld </div>
};

```
We should render it into the dom `Index.re`:
```reasonml
ReactDOMRe.renderToElementWithId(<HelloWorldReact />, "index");
```

And create a simple html to visualize it:
`index.html`:
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



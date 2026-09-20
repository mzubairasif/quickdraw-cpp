# Project Proposal — Neural Network from Scratch in C++

**Course:** OOP & Design Methodologies  
**Group Members:** [Name 1] · [Name 2] · [Name 3]  
**Date:** September 2026

---

## What We're Building

A neural network implemented entirely from scratch in C++, trained on Google's Quick Draw dataset — a collection of 50 million hand-drawn sketches across 345 categories. The network learns to classify hand-drawn shapes in real time.

The deliverable is a live demo: a browser canvas where a user draws a shape, and the system predicts what it is.

---

## Why This Project

Most machine learning work today happens through high-level libraries like PyTorch or TensorFlow, where the underlying math is completely abstracted away. We want to build the whole thing from the ground up — matrix operations, forward pass, backpropagation, weight updates — in C++, without any ML library. This forces a deep understanding of what these systems are actually doing, and gives us a genuine engineering problem to solve rather than a configuration exercise.

It is also a natural fit for OOP. The project decomposes cleanly into a set of well-defined classes with clear responsibilities, interfaces, and relationships — which is exactly what the course is about.

---

## Technical Scope

The system has three layers:

**Math engine** — a `Matrix` class in C++ implementing all operations the network depends on: multiplication, transpose, elementwise operations, scalar scaling, and axis-wise summation.

**Network core** — a layer-based neural network with abstract base classes for layers, activations, and loss functions. Concrete implementations include dense layers, ReLU, Sigmoid, Softmax, and cross-entropy loss. Forward pass and backpropagation are implemented from scratch using the chain rule.

**Infrastructure** — a data loader that parses Quick Draw's `.npy` files and feeds batches into the trainer. An SGD optimizer and training loop that runs epochs, tracks loss and accuracy, and saves weights to disk. A Python FastAPI server exposing a `/predict` endpoint, called from a browser canvas demo.

---

## OOP Design

The project uses the following design patterns:

| Pattern | Application |
|---|---|
| Strategy | Activation functions (ReLU / Sigmoid / Softmax interchangeable behind one interface) |
| Strategy | Loss functions (CrossEntropy / MSE interchangeable) |
| Strategy | Optimizer (SGD, extensible to Adam) |
| Template Method | Abstract `Layer` base class; `DenseLayer` as concrete implementation |
| Iterator | `DataLoader` batching |
| Builder | Network construction — layers added one by one |
| Singleton | `Config` object for hyperparameters |

Core OOP concepts present: inheritance, abstract classes, pure virtual functions, polymorphism, operator overloading, encapsulation, and composition.

Total classes: 15.

---

## Work Split

| Module | Responsibility |
|---|---|
| Person 1 | Matrix Engine — `Matrix` class, all math operations, operator overloading, weight serialisation, crossentropy, softmax, isolation testing |
| Person 2 | Network Core — layers, activations, loss functions, forward pass, backpropagation |
| Person 3 | Infrastructure — DataLoader, Trainer, SGD, FastAPI server, browser demo |

Persons 1 and 3 work in parallel from week one. Person 2 begins network architecture using a stub Matrix class and integrates the real engine once Person 1's handoff is complete.

---

## Timeline

| Week | Milestone |
|---|---|
| 1–2 | Matrix Engine core methods + DataLoader + network architecture design |
| 3–4 | Full Matrix Engine handoff + backpropagation implementation + Trainer |
| 5–6 | Integration, first training run, debugging, accuracy targets |
| 7 | FastAPI server + browser canvas demo |
| 8 | Polish, accuracy tuning, presentation |

---

## Success Criteria

- Network trains successfully on Quick Draw data with accuracy above 80%
- Live browser demo functional end to end
- All design patterns documented and demonstrable
- No external ML libraries — all math and training logic written in C++

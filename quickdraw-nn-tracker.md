# Quick Draw Neural Network — Project Tracker

**Course:** OOP & Design Methodologies  
**Last updated:** September 2026  
**Members:** Person 1 · Person 2 · Person 3 (fill names in)

---

## Decisions Log

| Date | Decision |
|---|---|
| Sep 2026 | CrossEntropyLoss and Softmax moved from Person 2 (Network Core) to Person 1 (Matrix Engine) — both are math-first components, natural fit |
| Sep 2026 | All three members are learning neural networks as they build — learning overhead factored into timeline |
| Sep 2026 | Timeline is 6 weeks — pre-work (3b1b series, Softmax reading) must happen before Week 1 starts |
| Sep 2026 | Mids fall in Weeks 3–4 — those weeks reduced to minimum viable work only; integration crunch shifted to Weeks 5–6 |
| Sep 2026 | Demo and presentation have no fixed timeline — will be scheduled separately when due date is known |
| Sep 2026 | Matrix internal storage: flat `std::vector<float>`, row-major, access as `data[row * cols + col]` |
| Sep 2026 | Fixed constants: batch_size = 32, input_dim = 784, num_classes = 345 |
| Sep 2026 | Person 2 to write plain-English Trainer comment block for Person 3 before Week 5 |
| Sep 2026 | Person 3 must run .npy sanity print after parsing — catches silent data corruption before training |

---

## Module Ownership (Updated)

### Person 1 — Math & Output Engine

**Owns:**
- `Matrix` class — all operations (multiply, transpose, elementwise\_multiply, add, subtract, scalar\_multiply, sum, operator overloading)
- `Softmax` — forward and backward pass
- `CrossEntropyLoss` — scalar loss computation + gradient w.r.t. output

**Why this grouping:** Softmax and CrossEntropyLoss are fundamentally math operations on vectors/matrices. CrossEntropy's gradient w.r.t. Softmax output is a well-known closed form (ŷ − y) — Person 1 implementing both keeps the math self-contained and off Person 2's plate.

**Deliverable:** `Matrix.h/cpp`, `Softmax.h/cpp`, `CrossEntropyLoss.h/cpp` — all tested with known inputs before handoff.

---

### Person 2 — Network Core (Lighter Load)

**Owns:**
- `Layer` (abstract base)
- `DenseLayer` — forward and backward pass
- `ReLU`, `Sigmoid` — forward and backward
- `NeuralNetwork` — forward pass, backward pass, layer orchestration

**No longer owns:** Softmax, CrossEntropyLoss (moved to Person 1)

**Stub strategy:** Person 2 uses a fake `Matrix` class returning hardcoded values from week 1. Swaps in real Matrix at Person 1's handoff (end of week 3 target). Person 2 needs to study backprop before implementing — start with 3Blue1Brown playlist then Karpathy's micrograd.

---

### Person 3 — Infrastructure

**Owns:**
- `DataLoader` — .npy parser, normalisation (0–255 → 0.0–1.0), train/val split (80/20), shuffle + batch
- `Optimizer` (abstract) + `SGD` — W = W − lr·grad\_W per layer
- `Trainer` — epoch loop, loss/accuracy tracking, saves best weights
- Weight serialisation — `save_weights()` / `load_weights()` binary I/O
- FastAPI `/predict` endpoint (Python)
- Browser canvas demo (HTML/JS) — draw → downsample to 28×28 → POST → live label

**Independent from day one.** No dependency on Person 1 or 2.

---

## Parallelization Strategy

The pipeline is sequential at the *data flow* level but not at the *build* level. Here's how all three work simultaneously from week 1:

```
Week 1–2:

Person 1:  [Matrix.h interface defined] → [multiply() + transpose()]
                                                      ↓
Person 2:  [Stub Matrix] → [Layer + DenseLayer forward] → [ReLU/Sigmoid interfaces]
                                                      ↑
Person 3:  [DataLoader] → [.npy parser] → [batch output shape defined]
                ↑
           Defines the (batch_size × 784) shape everything else trains on
           Person 3 outputs tell Person 2 exactly what input dimensions to expect
```

**The three sync points (where work merges):**

| Sync | When | What happens |
|---|---|---|
| Sync A | End of Week 4 | Person 1 hands off real Matrix + Softmax + CrossEntropyLoss to Person 2. Person 2 swaps stub. |
| Sync B | Mid-Week 5 | Person 3's Trainer hooks into Person 2's NeuralNetwork. First training run attempted. |
| Sync C | End of Week 6 | FastAPI + canvas connects to trained model. End-to-end demo running. |

---

## Interface Contracts

**Rule:** The person who *produces* data owns the signature. The person who *consumes* it accepts it — but both agree on the signature before either writes a line. Three handshakes to make in Week 1, ideally in a single 20-minute meeting.

---

### Handshake 1 — Person 1 → Person 2: Matrix

Person 1 writes `Matrix.h` first with the class structure and all method signatures — no implementation yet. Person 2 copies it verbatim as their stub header, filling each method body with a hardcoded return value. When Person 1 hands off the real implementation, Person 2 drops it in and nothing else changes.

**Internal storage decision (make this in Week 1, never revisit):** flat `std::vector<float>` in row-major order. Elements accessed as `data[row * cols + col]`. No pointer arithmetic, no 2D arrays, no raw `new`/`delete`. This is the simplest layout that works correctly.

```cpp
class Matrix {
public:
    int rows, cols;
    std::vector<float> data; // row-major: element (r,c) → data[r * cols + c]

    Matrix(int rows, int cols);          // constructor, initialise data to zeros
    void print() const;                  // debug utility

    Matrix multiply(const Matrix& B) const;
    Matrix transpose() const;
    Matrix elementwise_multiply(const Matrix& B) const;
    Matrix add(const Matrix& B) const;
    Matrix subtract(const Matrix& B) const;
    Matrix scalar_multiply(float k) const;
    Matrix sum(int axis) const;          // axis=0: sum rows → (1×cols), axis=1: sum cols → (rows×1)

    Matrix operator*(const Matrix& B) const;
    Matrix operator+(const Matrix& B) const;
    Matrix operator-(const Matrix& B) const;
};
```

---

### Handshake 2 — Person 2 → Person 3: NeuralNetwork

Person 2 writes and publishes these two signatures. Person 3's Trainer calls exactly these — it never touches layer internals, never accesses weights directly, never needs to know how many layers there are.

```cpp
// Forward pass: takes one batch as input, returns predictions (probabilities after Softmax)
// Input shape:  [batch_size × 784]
// Output shape: [batch_size × num_classes]  (num_classes = 345 for Quick Draw)
Matrix NeuralNetwork::forward(const Matrix& input);

// Backward pass: takes loss gradient, propagates through all layers in reverse
// Gradients are stored inside each layer — Trainer doesn't see them directly
// SGD then reads grad_W and grad_b from each layer via a separate update call
void NeuralNetwork::backward(const Matrix& loss_grad);
```

**Person 2 also writes this plain-English comment block for Person 3** — paste it at the top of `Trainer.cpp`:

```
// The training loop in plain English:
//   1. Get a batch from DataLoader  →  Matrix input [batch_size × 784], labels [batch_size × 1]
//   2. NeuralNetwork::forward(input)  →  predictions [batch_size × 345]
//   3. CrossEntropyLoss::compute(predictions, labels)  →  scalar loss (for logging)
//   4. CrossEntropyLoss::gradient(predictions, labels)  →  loss_grad [batch_size × 345]
//   5. NeuralNetwork::backward(loss_grad)  →  gradients stored inside layers
//   6. SGD::update(network)  →  applies W = W - lr * grad_W to every layer
//   Repeat for each batch. One full pass through the data = one epoch.
```

Person 3 implements that sequence. They don't need to understand why it works — just that sequence, in that order, every batch.

---

### Handshake 3 — Person 3 → Person 2: DataLoader output shape

One number: `batch_size`. Agree on 32 as the default. Person 3's `nextBatch()` returns a Matrix with 32 rows and 784 columns. Person 2 initialises DenseLayer's weight matrix W as `[hidden_size × 784]` accordingly. Write this down: **batch\_size = 32, input\_dim = 784, num\_classes = 345.**

```cpp
// Returns next batch of samples. Call repeatedly until exhausted, then reset for next epoch.
// Output: Matrix [batch_size × 784] — each row is one flattened 28×28 image, values in [0.0, 1.0]
Matrix DataLoader::nextBatch();

// Returns corresponding labels for the last batch returned by nextBatch()
// Output: Matrix [batch_size × 1] — integer class indices as floats (0 to 344)
Matrix DataLoader::nextLabels();
```

---

### Person 3 — Known risks and mitigations

Person 3's module is the most forgiving in the project (no backprop, no heavy math), but two things can go wrong quietly:

**Risk 1 — .npy parser producing silent garbage.** If the header offset is wrong or the shape dimensions are read backwards, DataLoader will produce garbage matrices and the network will train on nonsense with no error. Mitigation: after parsing, print the first sample's pixel values and visually verify — should be mostly zeros with some non-zero values, all between 0.0 and 1.0. One print statement catches this entirely.

```cpp
// Add this sanity check right after parsing:
Matrix sample = loader.nextBatch();
std::cout << "Sample[0] first 10 pixels: ";
for (int i = 0; i < 10; i++) std::cout << sample.data[i] << " ";
std::cout << std::endl;
// Expected output: something like  0 0 0.12 0.45 0.9 0.3 0 0 0 0
// Red flag: all zeros, all ones, values > 1.0, or values not in [0,1]
```

**Risk 2 — Trainer wiring going wrong without understanding why.** Person 3 doesn't need to understand backprop to implement the Trainer correctly — they just need to follow the comment block Person 2 writes (above). If training doesn't converge, the bug is almost certainly in Person 2's backward pass or Person 1's Matrix ops, not in Person 3's loop. Person 3's job in debugging is to confirm the loop order is correct and that they're passing the right matrices to the right functions.

---

## Milestone Schedule

**Timeline:** 6 weeks of project work. Demo and presentation scheduled separately.  
**Pre-work (before Week 1 starts):** Person 2 watches 3Blue1Brown neural network series (all 4 episodes). Person 1 reads up on Softmax math. Non-negotiable — there is no room to learn these in-week.  
**Mids note:** Weeks 3–4 are light by design. Only work that requires zero concentration (plumbing, testing, reading) is scheduled here. The crunch is in Weeks 5–6, after mids.

---

### Biweekly Milestones

#### Weeks 1–2 — Build in parallel, lock interfaces
*Target: ~4 hrs/week each. Front-load as much as possible — this is your last fully free stretch.*

**Person 1**
- [ ] Define `Matrix.h` — all method signatures, internal layout decided (row-major `vector<float>`)
- [ ] Implement and test `multiply()` with verified known inputs/outputs — most critical method in the project
- [ ] Implement `transpose()`
- [ ] Implement `elementwise_multiply()`, `add()`, `subtract()`, `scalar_multiply()`, `sum(axis)`
- [ ] Add operator overloading: `operator*`, `operator+`, `operator-`
- [ ] Begin `Softmax` — understand the math (exp normalization), write `forward()` if time allows

**Person 2**
- [ ] Write stub `Matrix` class — all signatures matching Person 1's `Matrix.h`, returning hardcoded values
- [ ] Define and implement `Layer` abstract base class
- [ ] Implement `DenseLayer::forward()` on stub Matrix — `Z = W·X + b`
- [ ] Define `Activation` abstract base + `ReLU` and `Sigmoid` with forward stubs
- [ ] Implement `NeuralNetwork` class shell — `vector<Layer*>`, `forward()` chain working end-to-end on stub
- [ ] Begin studying backprop chain rule — work through at least one manual example on paper

**Person 3**
- [ ] Read `.npy` format spec and implement parser — extract shape + raw float data
- [ ] Implement normalisation (0–255 → 0.0–1.0) and train/val split (80/20)
- [ ] Implement shuffle and batching — `nextBatch()` returns `[batch_size × 784]`
- [ ] Implement `Optimizer` abstract base + `SGD::update(layer)` — W = W − lr·grad\_W
- [ ] Agree `DataLoader::nextBatch()` output shape with Person 2 — write it down

**End of Week 2 checkpoint:** All three have something runnable. Interface contract locked. Person 1 has all Matrix ops done or nearly done. Person 2's forward pass works end-to-end on the stub.

---

#### Weeks 3–4 — Mids weeks. Minimum viable work only.
*Target: ~2 hrs/week each. No new difficult concepts. Only finish what's already started or write plumbing.*

**Person 1**
- [ ] Full isolation tests on all Matrix methods — known input → known output for every single method
- [ ] Implement `Softmax::forward()` and `Softmax::backward()` (math is manageable, no integration needed)
- [ ] Implement `CrossEntropyLoss::compute()` and `CrossEntropyLoss::gradient()` — gradient is just `ŷ − y`
- [ ] These three tasks above can be done in short focused sessions; save handoff for Week 5

**Person 2**
- [ ] Implement `ReLU::backward()` and `Sigmoid::backward()` — straightforward derivatives, doable in short sessions
- [ ] Read through backprop chain rule again with fresh eyes after mids — don't implement yet, just make sure it's solid conceptually
- [ ] `DenseLayer::backward()` is optional this fortnight — only attempt if mids go well and time allows

**Person 3**
- [ ] Build `Trainer` skeleton — epoch loop and batch loop structure only, no wiring to NeuralNetwork yet
- [ ] Implement `save_weights()` / `load_weights()` binary I/O — pure file handling, no math
- [ ] These are the lowest-concentration tasks in the whole project; ideal for mids period

**End of Week 4 checkpoint:** Person 1's math engine is complete and tested. Person 3 has Trainer skeleton and serialisation done. Person 2 has activations done. Nobody is blocked going into Week 5.

---

#### Weeks 5–6 — The real crunch. Integration, training run, demo.
*Target: ~5 hrs/week each. This is where the project actually comes together.*

**Person 1**
- [ ] **Handoff to Person 2 at the start of Week 5** — share `Matrix.h/cpp`, `Softmax.h/cpp`, `CrossEntropyLoss.h/cpp`
- [ ] Debug support throughout — shape mismatch errors, precision issues, any Matrix bugs Person 2 surfaces
- [ ] Polish code — comments, clean headers, readable variable names

**Person 2**
- [ ] Implement `DenseLayer::backward()` — dL/dW, dL/db, dL/dX using chain rule (this is the week for it)
- [ ] Implement `NeuralNetwork::backward()` — gradient flow through all layers in reverse
- [ ] **Swap stub for real Matrix from Person 1** — fix any signature mismatches
- [ ] Wire Softmax + CrossEntropyLoss into NeuralNetwork output end
- [ ] Verify: forward pass → valid probability distribution; backward pass → finite gradients of correct shapes
- [ ] Sanity check: loss decreases over 3–5 manual gradient steps before handing off to Trainer

**Person 3**
- [ ] Wire `Trainer` into Person 2's `NeuralNetwork` — complete the epoch + batch loop with forward → loss → backward → SGD
- [ ] Add loss and accuracy logging per epoch
- [ ] **First training run mid-Week 5** — target: loss going down, not crashing
- [ ] Build FastAPI `/predict` endpoint — load `model.bin`, run inference, return class label as JSON
- [ ] Build browser canvas demo — draw → 1-second timer → downsample to 28×28 → POST → live label update
- [ ] Confidence threshold: show "keep drawing..." if `max(softmax output) < 0.70`; skip inference on blank canvas

**End of Week 6 checkpoint:** Model trains and loss decreases. End-to-end demo works — draw in browser, get a label back. Code is clean. Project is done.

---

---

## Time Estimate

| Phase | Hrs/week per person | Notes |
|---|---|---|
| Weeks 1–2 | ~4 hrs | Front-load. Last free stretch before mids approach. |
| Weeks 3–4 | ~2 hrs | Mids. Minimum viable tasks only. |
| Weeks 5–6 | ~5 hrs | The real crunch. Integration + training run + demo. |
| **Total** | **~22 hrs per person** | **6 weeks** |

**What makes this feasible:** Pre-work (Person 2's 3b1b viewing) + the fact that Weeks 1–2 are genuinely productive before mids hit. If those two weeks are strong, Weeks 3–4 can be light without the project falling behind.

**What kills it:** Weeks 1–2 being wasted. If the team coasts early, there's no recovery — Weeks 3–4 can't absorb the load during mids, and Weeks 5–6 become impossible. Start strong.

---


---

## Classes Checklist (15 total)

- [ ] `Matrix` — Person 1
- [ ] `Softmax` — Person 1 *(moved from Person 2)*
- [ ] `CrossEntropyLoss` — Person 1 *(moved from Person 2)*
- [ ] `Layer` (abstract) — Person 2
- [ ] `DenseLayer` — Person 2
- [ ] `Activation` (abstract) — Person 2
- [ ] `ReLU` — Person 2
- [ ] `Sigmoid` — Person 2
- [ ] `NeuralNetwork` — Person 2
- [ ] `LossFunction` (abstract) — Person 2
- [ ] `DataLoader` — Person 3
- [ ] `Optimizer` (abstract) — Person 3
- [ ] `SGD` — Person 3
- [ ] `Trainer` — Person 3
- [ ] `Config` (Singleton) — Person 3

---

## Design Patterns Checklist (7 total)

- [ ] Strategy — Activation functions (ReLU/Sigmoid/Softmax same interface)
- [ ] Strategy — Loss functions (CrossEntropy/MSE same interface)
- [ ] Strategy — Optimizer (SGD, extensible to Adam)
- [ ] Template Method — Layer base → DenseLayer
- [ ] Iterator — DataLoader batching
- [ ] Builder — NeuralNetwork layer-by-layer construction
- [ ] Singleton — Config/hyperparameters

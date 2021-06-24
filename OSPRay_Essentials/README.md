# Intel&reg; Rendering Toolkit (Render Kit) Jupyter Notebook Training Content
This repo contains Jupyter Notebook Trainings for Render Kit that can be used on the Intel DevCloud for hands-on workshops and other training.

### Folder Structure
- [__release__](release) - Content that is reviewed and ready for use
- [__future__](future) - New content in progress
- __deprecated__ - Deprecated content

## Adding your Render Kit Jupyter Notebook Training to this repo:

- Follow the [Jupyter Notebook Playbook](https://gitlab.devtools.intel.com/ecosystem-dev-programs/playbooks/-/tree/master/creating-jupyter-notebooks) and create your Jupyter Notebook training.
- Create a merge request by adding your Jupyter notebook content to the `future` directory 
- Get your notebook reviewed by __Render Kit Training Team__
- When ready for external customers, move from _future_ to _release_ folder

## Creating Merge Request to this repo
These steps below will help you to push your changes and create merge request.
```
git clone https://gitlab.com/renderkit-training/renderkit-notebooks.git
cd renderkit-notebooks
```
Create a branch with any name _username-topic_:
```
git checkout -b username-topic
```
Copy your Jupyter notebook content into future directory:
```
cd future
cp <path-to-content>/course-name .
```
Push your branch to the repo
```
git add course-name/*
git commit -m "added course-name notebook files"
git push origin username-topic
```
You will see a notification in gitlab to create a merge request, complete the Merge Request
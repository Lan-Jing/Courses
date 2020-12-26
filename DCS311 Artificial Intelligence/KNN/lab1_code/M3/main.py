# this is the train-and-test module with different K parameters
#%%
import regression as rg

# define the hyper-parameter K
K = 5

model, validationAccuracy = rg.KNN_train("train_set.csv", "validation_set.csv", K)
testAccuracy = rg.KNN_test("train_set.csv", "test_set.csv", model, K)
# %%

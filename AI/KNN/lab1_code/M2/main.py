# this is the train-and-test module with different K parameters
#%%
import classification as cf

# define the hyper-parameter K
K = 10

model, validationAccuracy = cf.KNN_train("train_set.csv", "validation_set.csv", K)
testAccuracy = cf.KNN_test("train_set.csv", "test_set.csv", model, K)
# sampleAccuracy = cf.KNN_test("train_set.csv", "regression_simple_test.csv", model, K)

# %%
